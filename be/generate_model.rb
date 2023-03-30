#!/usr/bin/ruby

module Attrib
  ID = 2**0  # field: an id field for another pointer field object
  FIND = 2**1  # field: add a find method for the field
  PTR = 2**2  # field: field is a pointer to the object
  SAVEID = 2**3  # class: saving the object must explicitly set id
  TRANSIENT = 2**4  # field: not persistent
  KEY2 = 2**5  # field: secondary key, can be multiple fields which make it up
  NON_NULLABLE = 2**6 # field: if this field is not nullable. Note that this does not mean required. in fact, required fields should be allowed to be set to null so a proper error by the database is thrown.
  JOINTABLE = 2**7 # field: if this field is to a many-to-many join table [unused]
  DELETABLE = 2**8 # class: create delete method
end

######################### helpful functions & globals

# field items
$type = 0
$name = 1
$attrib = 2

def cap (x)
  if (x[0,2] == "re" && x[2,1] == x[2,1].upcase)
    return x.slice(0,2).upcase + x.slice(2..-1)
  else
    return x.slice(0,1).capitalize + x.slice(1..-1)
  end
end

def getVectorGeneric(t)
  return t.match(/^vector<(const )?([^*]*)\*?\>$/).captures[1]
end

def getSetGeneric(t)
  return t.match(/^set<(const )?([^*]*)\*?\>$/).captures[1]
end

def isVector(t)
  return t =~ /^vector</
end

def isSet(t)
  return t =~ /^set</
end

def single(n)
  if (n.downcase == "mixes")
    return "mix"
  elsif (n.downcase == "children")
    return "child"
  elsif (n.downcase == "playlistentries")
    return "playlistEntry"
  else
    return n[0..-2]
  end
end

def plural(x)
  if (x.downcase == "mix")
    return "mixes"
  elsif (x.downcase == "child")
    return "children"
  elsif (x.downcase == "rexml")
    return "reXml"
  elsif (x.downcase == "playlistentry")
    return "playlistEntries"
  else
    return "#{x}s"
  end
end

def vectorIds(f)
  "#{single(f[$name])}Ids"
end

def vectorObjs(f)
  "#{plural(f[$name][0..-4])}"
end

def vectorRelationTable(name, f)
  t = []
  if (f[$name].eql?("parents"))
    t[0] = "#{cap(name)}Children"
    t[1] = "parentId"
    t[2] = "childId"
  elsif (f[$name].eql?("children"))
    t[0] = "#{cap(name)}Children"
    t[1] = "childId"
    t[2] = "parentId"
  elsif (f[$attrib] & Attrib::JOINTABLE > 0)
    t[0] = "#{cap(plural(getVectorGeneric(f[$type])))}"
    t[1] = "#{single(f[$name])}Id"
    t[2] = "#{name}Id"
  else
    t[0] = "#{cap(name)}#{cap(plural(getVectorGeneric(f[$type])))}"
    t[1] = "#{single(f[$name])}Id"
    t[2] = "#{name}Id"
  end
  return t
end

def selectStar(name, fields)
  str = "#{cap(plural(name))}.*";
  fields.select{|f| isVector(f[$type]) && f[$attrib] & Attrib::ID > 0}.each do |f|
    str << ", group_concat(distinct(#{plural(f[$name][0..-4])}.#{f[$attrib] & Attrib::JOINTABLE > 0 ? 'id' : single(f[$name])})) as #{f[$name]}"
  end
  return str
end

def fromTable(name, fields)
  str = "#{cap(plural(name))}";
  fields.select{|f| isVector(f[$type]) && f[$attrib] & Attrib::ID == 0}.each do |f|
    t = vectorRelationTable(name, f)
    str << " left outer join #{t[0]} #{f[$name]} on #{cap(plural(name))}.id = #{f[$name]}.#{t[2]}"
  end
  return str
end

def groupBy(name, fields)
  fields.select{|f| isVector(f[$type]) && f[$attrib] & Attrib::ID == 0}.each do |f|
    return " group by #{cap(plural(name))}.id"
  end
  return ""
end

def setField(f, i, indent)
  str = ""
  if (f[$type] == :bool)
    str << indent << "ps->set#{cap(f[$type].to_s)}ean(#{i}, #{f[$name]});\n"
  elsif (f[$type] == :time_t)
    str << indent << "ps->setString(#{i}, stringFromTime(#{f[$name]}));\n"
  elsif (isSet(f[$type]))
    str << indent << "if (#{f[$name]}.size() > 0) ps->set#{cap(getSetGeneric(f[$type]))}(#{i}, setToCsv(#{f[$name]}));\n"
    str << indent << "else ps->setNull(#{i}, sql::DataType::SET);\n"
  elsif (f[$type] == :int && f[$attrib] & Attrib::NON_NULLABLE > 0)
    str << indent << "ps->set#{cap(f[$type].to_s)}(#{i}, #{f[$name]});\n"
  elsif (f[$type] == :int)
    str << indent << "if (#{f[$name]} > 0) ps->set#{cap(f[$type].to_s)}(#{i}, #{f[$name]});\n"
    str << indent << "else ps->setNull(#{i}, sql::DataType::INTEGER);\n"
  elsif (f[$attrib] & Attrib::NON_NULLABLE > 0)
    str << indent << "ps->set#{cap(f[$type].to_s)}(#{i}, #{f[$name]});\n"
  else
    str << indent << "if (!#{f[$name]}.empty()) ps->set#{cap(f[$type].to_s)}(#{i}, #{f[$name]});\n"
    str << indent << "else ps->setNull(#{i}, sql::DataType::VARCHAR);\n"
  end
  return str
end

def defaultNullValue(f)
  if (f[$type] == :bool)
    return "false"
  elsif (f[$type] == :time_t)
    return "0"
  elsif (isSet(f[$type]))
    return "''"
  elsif (f[$type] == :int)
    return "0"
  else
    return "''"
  end
end

######################### h & cc outputs

def sqlTryBlock()
  return "        for (int i = 0; i < 2; ++i) {\n            try {\n"
end

def sqlCatchBlock()
  return "            } catch (sql::SQLException &e) {\n                LOG(WARNING) << \"ERROR: SQLException in \" << __FILE__ << \" (\" << __func__<< \") on line \" << __LINE__;\n                LOG(WARNING) << \"ERROR: \" << e.what() << \" (MySQL error code: \" << e.getErrorCode() << \", SQLState: \" << e.getSQLState() << \")\";\n                bool reconnected = MysqlAccess::getInstance().reconnect();\n                LOG(INFO) << (reconnected ? \"Successful\" : \"Failed\") << \" mysql reconnection\";\n            }\n        }\n        LOG(FATAL) << \"Unable to complete model operation\";\n"
end

def syncChildBlock(f)
  return "                if (#{f[$name]} && #{f[$name]}->sync()) {\n                    if (#{f[$name]}->getId()) {\n                        #{f[$name]}->update();\n                    } else {\n                        #{f[$name]}->save();\n                    }\n                    #{f[$name]}Id = #{f[$name]}->getId();\n                } else if (!#{f[$name]}Id && #{f[$name]}) {\n                    #{f[$name]}Id = #{f[$name]}->getId();\n                }\n"
end

def hFieldDeclaration(f)
  str = "        #{f[$type]}#{'*' if (f[$attrib] & Attrib::PTR > 0)} #{f[$name]};"
  str << "  // transient" if (f[$attrib] & Attrib::TRANSIENT > 0)
  str << "\n"
end

def hConstructor(name)
  return "        #{cap(name)}();\n"
end

def cConstructor(name, fields)
  str = "    #{cap(name)}::#{cap(name)}() :\n"
  fields.each do |f|
    if (f[$type] == :int)
      str << "    #{f[$name]}(0),\n"
    elsif (f[$type] == :bool)
      str << "    #{f[$name]}(false),\n"
    elsif (f[$attrib] & Attrib::PTR > 0 || f[$type] == :time_t)
      str << "    #{f[$name]}(NULL),\n"
    else
      str << "    #{f[$name]}(),\n"
    end
  end
  str = str[0..-3]
  str << " {\n    }\n\n"
end

def hCopyConstructor(name)
  return "        explicit #{cap(name)}(const #{cap(name)}& #{name});\n"
end

def cCopyConstructor(name, fields)
  str = "    #{cap(name)}::#{cap(name)}(const #{cap(name)}& #{name}) :\n"
  fields.each do |f|
    if (f[$attrib] & Attrib::PTR > 0)
      str << "    #{f[$name]}(NULL),\n"
    elsif (isVector(f[$type]) && f[$attrib] & Attrib::ID == 0)
      str << "    #{f[$name]}(),\n"
    else
      str << "    #{f[$name]}(#{name}.get#{cap(f[$name])}()),\n"
    end
  end
  str = str[0..-3]
  str << " {\n"
  fields.each do |f|
    if (f[$attrib] & Attrib::PTR > 0)
      str << "        if (#{name}.#{f[$name]}) set#{cap(f[$name])}(*#{name}.#{f[$name]});\n"
    end
  end
  str << "    }\n\n"
end

def hAssignmentConstructor(name)
  return "        void operator=(const #{cap(name)}& #{name});\n"
end

def cAssignmentConstructor(name, fields)
  str = "    void #{cap(name)}::operator=(const #{cap(name)}& #{name}) {\n"
  fields.each do |f|
    if (f[$attrib] & Attrib::PTR > 0)
      str << "        if (!#{name}.get#{cap(f[$name])}Id() && #{name}.#{f[$name]}) {\n            if (!#{f[$name]}) #{f[$name]} = new #{f[$type]}(*#{name}.#{f[$name]});\n            else *#{f[$name]} = *#{name}.#{f[$name]};\n        } else {\n            delete #{f[$name]};\n            #{f[$name]} = NULL;\n        }\n"
    elsif (isVector(f[$type]) && f[$attrib] & Attrib::ID == 0)
      str << "        deleteVectorPointers(&#{f[$name]});\n"
    else
      str << "        #{f[$name]} = #{name}.get#{cap(f[$name])}();\n"
    end
  end
  str << "    }\n\n"
end

def hDestructor(name)
  return "        ~#{cap(name)}();\n"
end

def cDestructor(name, fields)
  str = "    #{cap(name)}::~#{cap(name)}() {\n"
  fields.each do |f|
    if (f[$attrib] & Attrib::PTR > 0)
      str << "        delete #{f[$name]};\n"
      str << "        #{f[$name]} = NULL;\n"
    elsif (isVector(f[$type]) && f[$attrib] & Attrib::ID == 0)
      str << "        while (!#{f[$name]}.empty()) delete #{f[$name]}.back(), #{f[$name]}.pop_back();\n"
    end
  end
  str << "    }\n\n"
end

def hClearFunction()
  return "        void clear();\n"
end

def cClearFunction(name, fields)
  str = "    void #{cap(name)}::clear() {\n"
  fields.each do |f|
    if (f[$type] == :int || f[$type] == :time_t)
      str << "        #{f[$name]} = 0;\n"
    elsif (isVector(f[$type]) && f[$attrib] & Attrib::ID == 0)
      str << "        deleteVectorPointers(&#{f[$name]});\n"
    elsif (f[$type] == :string || isSet(f[$type]) || isVector(f[$type]))
      str << "        #{f[$name]}.clear();\n"
    elsif (f[$type] == :bool)
      str << "        #{f[$name]} = false;\n"
    elsif (f[$attrib] & Attrib::PTR > 0)
      str << "        delete #{f[$name]};\n"
      str << "        #{f[$name]} = NULL;\n"
    else
      str << "        // TODO #{f[$name]}\n"
    end
  end
  str << "    }\n\n"
end

def hFindFunction(name, f)
  if ([:int, :bool, :time_t].include?(f[$type]))
    return "        static #{cap(name)}* findBy#{cap(f[$name])}(#{f[$type]} #{f[$name]});\n"
  else
    return "        static #{cap(name)}* findBy#{cap(f[$name])}(const #{f[$type]}& #{f[$name]});\n"
  end
end

def cFindFunction(name, f, fields)
  str = ""
  if ([:int, :bool, :time_t].include?(f[$type]))
    str << "    #{cap(name)}* #{cap(name)}::findBy#{cap(f[$name])}(#{f[$type]} #{f[$name]}) {\n"
  else
    str << "    #{cap(name)}* #{cap(name)}::findBy#{cap(f[$name])}(const #{f[$type]}& #{f[$name]}) {\n"
  end
  str << sqlTryBlock()
  str << "                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement(\"select #{selectStar(name, fields)} from #{fromTable(name, fields)} where #{cap(plural(name))}.#{f[$name]} = ?#{groupBy(name,fields)}\");\n"
  str << "                ps->set#{cap(f[$type].to_s)}(1, #{f[$name]});\n                sql::ResultSet *rs = ps->executeQuery();\n                #{cap(name)} *#{name} = NULL;\n                if (rs->next()) {\n                    #{name} = new #{cap(name)}();\n                    populateFields(rs, #{name});\n                }\n                rs->close();\n                delete rs;\n\n"
  str << "                return #{name};\n"
  str << sqlCatchBlock()
  str << "    }\n\n"
end

def hSecondaryKeysFindFunction(name, secondaryKeys)
  str = "        static #{cap(name)}* findBy"
  secondaryKeys.each_with_index do |f,idx|
    if (idx > 0)
      str << "And"
    end
    str << "#{cap(f[$name])}"
  end
  str << "("
  secondaryKeys.each_with_index do |f,idx|
    if (idx > 0)
      str << ", "
    end
    if ([:int, :bool, :time_t].include?(f[0]))
      str << "#{f[$type]} #{f[$name]}"
    else
      str << "const #{f[$type]}& #{f[$name]}"
    end
  end
  str << ");\n"
  if (secondaryKeys.length > 1)
    secondaryKeys.each do |f|
      str << "        static ResultSetIterator<#{cap(name)}>* findBy#{cap(f[$name])}(#{f[$type]} #{f[$name]});\n"
    end
  end
  return str
end

def cSecondaryKeysFindFunction(name, secondaryKeys, fields)
  str = "    #{cap(name)}* #{cap(name)}::findBy"
  secondaryKeys.each_with_index do |f,idx|
    if (idx > 0)
      str << "And"
    end
    str << "#{cap(f[$name])}"
  end
  str << "("
  secondaryKeys.each_with_index do |f,idx|
    if (idx > 0)
      str << ", "
    end
    if ([:int, :bool, :time_t].include?(f[0]))
      str << "#{f[$type]} #{f[$name]}"
    else
      str << "const #{f[$type]}& #{f[$name]}"
    end
  end
  str << ") {\n"
  str << sqlTryBlock()
  str << "                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement(\"select #{selectStar(name, fields)} from #{fromTable(name, fields)} where "
  secondaryKeys.each_with_index do |f,idx|
    if (idx > 0)
      str << " and "
    end
    str << "ifnull(#{f[$name]}," << defaultNullValue(f) << ") = ifnull(?," << defaultNullValue(f) << ")"
  end
  str << "#{groupBy(name,fields)}\");\n"
  secondaryKeys.each_with_index do |f,idx|
    str << setField(f, idx+1, "                ")
  end
  str << "                sql::ResultSet *rs = ps->executeQuery();\n                #{cap(name)} *#{name} = NULL;\n                if (rs->next()) {\n                    #{name} = new #{cap(name)}();\n                    populateFields(rs, #{name});\n                }\n                rs->close();\n                delete rs;\n\n                return #{name};\n"
  str << sqlCatchBlock()
  str << "    }\n\n"
  if (secondaryKeys.length > 1)
    secondaryKeys.each do |f|
      str << "    ResultSetIterator<#{cap(name)}>* #{cap(name)}::findBy#{cap(f[$name])}(#{f[$type]} #{f[$name]}) {\n"
      str << sqlTryBlock()
      str << "                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement(\"select #{selectStar(name, fields)} from #{fromTable(name, fields)} where "
      str << "ifnull(#{f[$name]}," << defaultNullValue(f) << ") = ifnull(?," << defaultNullValue(f) << ")"
      str << "#{groupBy(name,fields)}\");\n" << setField(f, 1, "                ")
      str << "                sql::ResultSet *rs = ps->executeQuery();\n                ResultSetIterator<#{cap(name)}> *dtrs = new ResultSetIterator<#{cap(name)}>(rs);\n                return dtrs;\n"
      str << sqlCatchBlock()
      str << "    }\n\n"
    end
  end
  return str
end

def hFindAllFunction(name)
  return "        static ResultSetIterator<#{cap(name)}>* findAll();\n"
end

def cFindAllFunction(name, fields)
  str = "    ResultSetIterator<#{cap(name)}>* #{cap(name)}::findAll() {\n"
  str << sqlTryBlock()
  str << "                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement(\"select #{selectStar(name, fields)} from #{fromTable(name, fields)}#{groupBy(name,fields)}\");\n                sql::ResultSet *rs = ps->executeQuery();\n                ResultSetIterator<#{cap(name)}> *dtrs = new ResultSetIterator<#{cap(name)}>(rs);\n                return dtrs;\n"
  str << sqlCatchBlock()
  str << "    }\n"
end

def hSyncFunction()
  return "        // sync with db & return true if object needs saving or updating\n        bool sync();\n"
end

def cSyncFunction(name, fields, secondaryKeys)
  str = "    bool #{cap(name)}::sync() {\n        #{cap(name)}* #{name} = findById(id);\n"
  if (!fields.select{|f| f[$attrib] & Attrib::PTR > 0}.empty?)
    str << "        if (!#{name}) {\n"
    fields.select{|f| f[$attrib] & Attrib::PTR > 0}.each do |f|
      str << "            if (!#{f[$name]}Id && #{f[$name]}) {\n                #{f[$name]}->sync();\n                #{f[$name]}Id = #{f[$name]}->getId();\n            }\n"
    end
    str << "        }\n"
  end
  if (!secondaryKeys.empty?)
    str << "        if (!#{name}) #{name} = findBy"
    secondaryKeys.each_with_index do |f,idx|
      if (idx > 0)
        str << "And"
      end
      str << "#{cap(f[$name])}"
    end
    str << "("
    secondaryKeys.each_with_index do |f,idx|
      if (idx > 0)
        str << ", "
      end
      str << "get#{cap(f[$name])}()"
    end
    str << ");\n"
  end
  str << "        if (!#{name}) return true;\n\n"
  str << "        // check fields\n        bool needsUpdate = false;\n"
  str << "        boost::regex decimal(\"(-?\\\\d+)\\\\.?\\\\d*\");\n        boost::smatch match1;\n        boost::smatch match2;\n"
  fields.each do |f|
    if (f[$attrib] & Attrib::TRANSIENT > 0)
      next
    elsif ([:int, :bool].include?(f[$type]))
      str << "        if (#{f[$name]} != #{name}->get#{cap(f[$name])}()) {\n            if (#{f[$name]}) {\n"
      str << "                LOG(INFO) << \"updating #{name} \" << id << \" #{f[$name]} from \" << #{name}->get#{cap(f[$name])}() << \" to \" << #{f[$name]};\n                needsUpdate = true;\n            } else {\n"
      str << "                #{f[$name]} = #{name}->get#{cap(f[$name])}();\n            }\n        }\n"
    elsif ([:time_t].include?(f[$type]))
      str << "        if (#{f[$name]} != #{name}->get#{cap(f[$name])}()) {\n            if (!#{name}->get#{cap(f[$name])}()) {\n"
      str << "                LOG(INFO) << \"updating #{name} \" << id << \" #{f[$name]} from \" << #{name}->get#{cap(f[$name])}() << \" to \" << #{f[$name]};\n                needsUpdate = true;\n            } else {\n"
      str << "                #{f[$name]} = #{name}->get#{cap(f[$name])}();\n            }\n        }\n"
    elsif (f[$type] == :string)
      str << "        if (#{f[$name]}.compare(#{name}->get#{cap(f[$name])}())  && (!boost::regex_match(#{f[$name]}, match1, decimal) || !boost::regex_match(#{name}->get#{cap(f[$name])}(), match2, decimal) || match1[1].str().compare(match2[1].str()))) {\n"
      str << "            if (!#{f[$name]}.empty()) {\n"
      str << "                LOG(INFO) << \"updating #{name} \" << id << \" #{f[$name]} from \" << #{name}->get#{cap(f[$name])}() << \" to \" << #{f[$name]};\n                needsUpdate = true;\n            } else {\n"
      str << "                #{f[$name]} = #{name}->get#{cap(f[$name])}();\n            }\n        }\n"
    elsif (isVector(f[$type]) && f[$attrib] & Attrib::ID > 0)
      str << "        if (!equivalentVectors<int>(#{f[$name]}, #{name}->get#{cap(f[$name])}())) {\n            if (!containsVector<int>(#{f[$name]}, #{name}->get#{cap(f[$name])}())) {\n"
      str << "                LOG(INFO) << \"updating #{name} \" << id << \" #{f[$name]}\";\n                needsUpdate = true;\n            }\n"
      str << "            appendUniqueVector<int>(#{name}->get#{cap(f[$name])}(), &#{f[$name]});\n        }\n"
    elsif (isSet(f[$type]))
      str << "        if (!equivalentSets<#{getSetGeneric(f[$type])}>(#{f[$name]}, #{name}->#{f[$name]})) {\n            if (!containsSet<#{getSetGeneric(f[$type])}>(#{f[$name]}, #{name}->#{f[$name]})) {\n"
      str << "                LOG(INFO) << \"updating #{name} \" << id << \" #{f[$name]}\";\n                needsUpdate = true;\n            }\n"
      str << "            #{f[$name]}.insert(#{name}->#{f[$name]}.begin(), #{name}->#{f[$name]}.end());\n        }\n"
    elsif (f[$attrib] & Attrib::PTR > 0)
      str << "        if (#{f[$name]}) needsUpdate |= #{f[$name]}->sync();\n"
    else
      next
    end
  end
  str << "        return needsUpdate;\n    }\n\n"
end

def hUpdateFunction()
  return "        int update();\n"
end

def cUpdateFunction(name, fields)
  str = "    int #{cap(name)}::update() {\n"
  str << sqlTryBlock()
  fields.select{|f| f[$attrib] & Attrib::PTR > 0}.each do |f|
    str << syncChildBlock(f)
  end
  str << "\n"
  str << "                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement(\"update #{cap(plural(name))} set "
  fields.each do |f|
    next if (f[$name] == "id" || f[$attrib] & Attrib::PTR > 0 || isVector(f[$type]) || f[$attrib] & Attrib::TRANSIENT > 0)
    str << "#{f[$name]}=?, "
  end
  str = str[0..-3]
  str << " where id=?\");\n"
  i = 0
  fields.each do |f|
    next if (f[$name] == "id" || f[$attrib] & Attrib::PTR > 0 || isVector(f[$type]) || f[$attrib] & Attrib::TRANSIENT > 0)
    i += 1
    str << setField(f, i, "                ")
  end
  str << "                ps->setInt(#{i + 1}, id);\n                int result = ps->executeUpdate();\n"
  fields.select{|f| isVector(f[$type]) && f[$attrib] & Attrib::ID == 0}.each do |f|
    t = vectorRelationTable(name, f)
    str << "                if (!#{vectorIds(f)}.empty()) {\n"
    str << "                    stringstream ss(\"insert ignore into #{t[0]} (#{t[2]}, #{t[1]}) values (?, ?)\", ios_base::app | ios_base::out | ios_base::ate);\n                    for (int i = 1; i < #{vectorIds(f)}.size(); ++i) {\n                        ss << \", (?, ?)\";\n                    }\n                    ps = MysqlAccess::getInstance().getPreparedStatement(ss.str());\n"
    str << "                    for (int i = 0; i < #{vectorIds(f)}.size(); ++i) {\n                        ps->setInt(i * 2 + 1, id);\n                        ps->setInt(i * 2 + 2, #{vectorIds(f)}[i]);\n                    }\n                    ps->executeUpdate();\n"
    str << "                    ss.str(std::string());\n                    ss << \"delete ignore from #{t[0]} where #{t[2]} = ? and #{t[1]} not in (?\";\n                    for (int i = 1; i < #{vectorIds(f)}.size(); ++i) {\n                        ss << \", ?\";\n                    }\n                    ss << \")\";\n"
    str << "                    ps = MysqlAccess::getInstance().getPreparedStatement(ss.str());\n                    ps->setInt(1, id);\n                    for (int i = 0; i < #{vectorIds(f)}.size(); ++i) {\n                        ps->setInt(i + 2, #{vectorIds(f)}[i]);\n                    }\n                    ps->executeUpdate();\n"
    str << "                } else {\n                    ps = MysqlAccess::getInstance().getPreparedStatement(\"delete ignore from #{t[0]} where #{t[2]} = ?\");\n                    ps->setInt(1, id);\n                    ps->executeUpdate();\n                }\n"
  end
  str << "                return result;\n"
  str << sqlCatchBlock()
  str << "    }\n\n"
end

def hSaveFunction()
  return "        int save();\n"
end

def cSaveFunction(name, fields, attribs)
  str = "    int #{cap(name)}::save() {\n"
  str << sqlTryBlock()
  fields.select{|f| f[$attrib] & Attrib::PTR > 0}.each do |f|
    str << syncChildBlock(f)
  end
  if (attribs & Attrib::SAVEID > 0)
    str << "                if (id == 0) {\n                    sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement(\"select max(id) from #{cap(plural(name))}\");\n                    sql::ResultSet *rs = ps->executeQuery();\n                    rs->next();\n                    id = rs->getInt(1) + 1;\n                    rs->close();\n                    delete rs;\n                }\n"
  end
  str << "\n"
  str << "                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement(\"insert into #{cap(plural(name))} ("
  i = 0
  fields.each do |f|
    next if ((f[$name] == "id" && attribs & Attrib::SAVEID == 0) || f[$attrib] & Attrib::PTR > 0 || isVector(f[$type]) || f[$attrib] & Attrib::TRANSIENT > 0)
    i += 1
    str << "#{f[$name]}, "
  end
  str = str[0..-3]
  str << ") values ("
  (1..i-1).each do
    str << "?, "
  end
  str << "?)\");\n"
  i = 0
  fields.each do |f|
    next if ((f[$name] == "id" && attribs & Attrib::SAVEID == 0) || f[$attrib] & Attrib::PTR > 0 || isVector(f[$type]) || f[$attrib] & Attrib::TRANSIENT > 0)
    i += 1
    str << setField(f, i, "                ")
  end
  str << "                int saved = ps->executeUpdate();\n                if (!saved) {\n"
  str << "                    LOG(WARNING) << \"Not able to save #{name}\";\n                    return saved;\n"
  str << "                } else {\n"
  if (attribs & Attrib::SAVEID == 0)
    str << "                    id = MysqlAccess::getInstance().getLastInsertId();\n                    if (id == 0) {\n"
    str << "                        LOG(WARNING) << \"Inserted #{name}, but unable to retreive inserted ID.\";\n                        return saved;\n                    }\n"
  end
  fields.each do |f|
    if (isVector(f[$type]) && f[$attrib] & Attrib::ID == 0)
    t = vectorRelationTable(name, f)
    str << "                    if (!#{vectorIds(f)}.empty()) {\n                        stringstream ss(\"insert ignore into #{t[0]} (#{t[2]}, #{t[1]}) values (?, ?)\", ios_base::app | ios_base::out | ios_base::ate);\n                        for (int i = 1; i < #{vectorIds(f)}.size(); ++i) {\n                            ss << \", (?, ?)\";\n                        }\n                        ps = MysqlAccess::getInstance().getPreparedStatement(ss.str());\n"
    str << "                        for (int i = 0; i < #{vectorIds(f)}.size(); ++i) {\n                            ps->setInt(i * 2 + 1, id);\n                            ps->setInt(i * 2 + 2, #{vectorIds(f)}[i]);\n                        }\n                        if (!ps->executeUpdate()) {\n                            LOG(WARNING) << \"Did not save #{single(f[$name])} for #{name} \" << id;\n                        }\n                    }\n"
    end
  end
  str << "                    return saved;\n                }\n"
  str << sqlCatchBlock()
  str << "    }\n\n"
end

def hEraseFunction()
  return "        int erase();\n"
end

def cEraseFunction(name, fields)
  str = "    int #{cap(name)}::erase() {\n"
  str << sqlTryBlock()
  fields.each do |f|
    next unless (isVector(f[$type]) && !["int", "string"].include?(getVectorGeneric(f[$type])) && f[$attrib] & Attrib::JOINTABLE > 0)
    str << "                {\n                    const #{f[$type]}& #{f[$name]} = get#{cap(f[$name])}();\n                    for (#{f[$type]}::const_iterator it = #{f[$name]}.begin(); it != #{f[$name]}.end(); ++it) {\n                        (*it)->erase();\n                    }\n                    #{f[$type]} tmp;\n                    set#{cap(f[$name])}(tmp);\n                }\n"
  end
  str << "                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement(\"delete from #{cap(plural(name))} where id=?\");\n"
  str << "                ps->setInt(1, id);\n"
  str << "                int erased = ps->executeUpdate();\n                if (!erased) {\n"
  str << "                    LOG(WARNING) << \"Not able to erase #{name}\";\n"
  str << "                }\n"
  str << "                return erased;\n"
  str << sqlCatchBlock()
  str << "    }\n\n"
end

def hPopulateFieldFunctions(name, fields)
  str = "        static void populateFields(const sql::ResultSet* rs, #{cap(name)}* #{name});\n"
  return str
end

def cPopulateFieldFunctions(name, fields)
  # populate basic fields
  str = "    void #{cap(name)}::populateFields(const sql::ResultSet* rs, #{cap(name)}* #{name}) {\n"
  fields.each do |f|
    next if (f[$attrib] & Attrib::PTR > 0 || f[$attrib] & Attrib::TRANSIENT > 0 || isVector(f[$type]) || isSet(f[$type]))
    if (f[$type] == :bool)
      str << "        #{name}->set#{cap(f[$name])}(rs->getBoolean(\"#{f[$name]}\"));\n"
    elsif (f[$type] == :time_t)
      str << "        #{name}->set#{cap(f[$name])}(timeFromString(rs->getString(\"#{f[$name]}\")));\n"
    else
      str << "        #{name}->set#{cap(f[$name])}(rs->get#{cap(f[$type].to_s)}(\"#{f[$name]}\"));\n"
    end
  end
  fields.select{|f| isVector(f[$type]) && f[$attrib] & Attrib::ID > 0}.each do |f|
    str << "        if (!rs->isNull(\"#{f[$name]}\")) {\n            string csv = rs->getString(\"#{f[$name]}\");\n            istringstream iss(csv);\n            string id;\n            while (getline(iss, id, ',')) {\n              #{name}->#{f[$name]}.push_back(atoi(id.c_str()));\n            }\n        }\n"
  end
  fields.select{|f| isSet(f[$type])}.each do |f|
    str << "        if (!rs->isNull(\"#{f[$name]}\")) {\n            #{getSetGeneric(f[$type])} dbSet = rs->getString(\"#{f[$name]}\");\n            boost::split(#{name}->#{f[$name]}, dbSet, boost::is_any_of(\",\"));\n        }\n"
  end
  str << "    }\n\n"
  return str
end

def hAccessor(f)
  str = ""
  if ([:int, :bool, :time_t].include?(f[$type]))
    str << "        const #{f[$type]} get#{cap(f[$name])}() const;\n"
    str << "        void set#{cap(f[$name])}(#{f[$type]} #{f[$name]});\n"
  elsif (f[$attrib] & Attrib::PTR > 0)
    str << "        #{f[$type]}* get#{cap(f[$name])}();\n"
    str << "        #{f[$type]}* get#{cap(f[$name])}Const() const;\n"
    str << "        void set#{cap(f[$name])}(const #{f[$type]}& #{f[$name]});\n"
    str << "        void set#{cap(f[$name])}(#{f[$type]}* #{f[$name]});  // takes ownership\n"
  elsif (isVector(f[$type]))
    if (f[$attrib] & Attrib::ID > 0)
      str << "        const #{f[$type]}& get#{cap(f[$name])}() const;\n"
    else
      str << "        const #{f[$type]}& get#{cap(f[$name])}();\n"
    end
    str << "        void set#{cap(f[$name])}(const #{f[$type]}& #{f[$name]});\n"
    #str << "        void add#{cap(single(f[$name]))}(const #{getVectorGeneric(f[$type])}& #{single(f[$name])});\n"
    #str << "        void remove#{cap(single(f[$name]))}(const #{getVectorGeneric(f[$type])}& #{single(f[$name])});\n"
  elsif (isSet(f[$type]))
    str << "        const #{f[$type]}& get#{cap(f[$name])}() const;\n"
    str << "        void set#{cap(f[$name])}(const #{f[$type]}& #{f[$name]});\n"
    str << "        void add#{cap(single(f[$name]))}(const #{getSetGeneric(f[$type])}& #{single(f[$name])});\n"
    str << "        void remove#{cap(single(f[$name]))}(const #{getSetGeneric(f[$type])}& #{single(f[$name])});\n"
  else
    str << "        const #{f[$type]}& get#{cap(f[$name])}() const;\n"
    str << "        void set#{cap(f[$name])}(const #{f[$type]}& #{f[1]});\n"
  end
  return str
end

def cAccessor(name, f)
  str = ""
  if (f[$type] == :string)
    str << "    const string& #{cap(name)}::get#{cap(f[$name])}() const { return #{f[$name]}; }\n"
    str << "    void #{cap(name)}::set#{cap(f[$name])}(const string& #{f[$name]}) { this->#{f[$name]} = #{f[$name]}; }\n"
  elsif (f[$attrib] & Attrib::PTR > 0)
    str << "    #{f[$type]}* #{cap(name)}::get#{cap(f[$name])}() {\n        if (!#{f[$name]} && #{f[$name]}Id) {\n            #{f[$name]} = #{f[$type][0..-1]}::findById(#{f[$name]}Id);\n        }\n        return #{f[$name]};\n    }\n"
    str << "    #{f[$type]}* #{cap(name)}::get#{cap(f[$name])}Const() const {\n        return (!#{f[$name]} && #{f[$name]}Id) ? #{f[$type][0..-1]}::findById(#{f[$name]}Id) : #{f[$name]};\n    }\n"
    str << "    void #{cap(name)}::set#{cap(f[$name])}(const #{f[$type]}& #{f[$name]}) {\n        this->#{f[$name]}Id = #{f[$name]}.getId();\n        delete this->#{f[$name]};\n        this->#{f[$name]} = new #{f[$type]}(#{f[$name]});\n    }\n"
    str << "    void #{cap(name)}::set#{cap(f[$name])}(#{f[$type]}* #{f[$name]}) {\n        this->#{f[$name]}Id = #{f[$name]}->getId();\n        delete this->#{f[$name]};\n        this->#{f[$name]} = #{f[$name]};\n    }\n"
  elsif (isVector(f[$type]) && f[$attrib] & Attrib::ID == 0)
    str << "    const #{f[$type]}& #{cap(name)}::get#{cap(f[$name])}() {\n        if (#{f[$name]}.empty() && !#{vectorIds(f)}.empty()) {\n            for (vector<int>::const_iterator it = #{vectorIds(f)}.begin(); it != #{vectorIds(f)}.end(); ++it) {\n                #{f[$name]}.push_back(#{getVectorGeneric(f[$type])}::findById(*it));\n            }\n        }\n        return #{f[$name]};\n    }\n"
    str << "    void #{cap(name)}::set#{cap(f[$name])}(const #{f[$type]}& #{f[$name]}) {\n        deleteVectorPointers<#{getVectorGeneric(f[$type])}*>(&this->#{f[$name]});\n        this->#{f[$name]} = #{f[$name]};\n        this->#{vectorIds(f)}.clear();\n        for (#{f[$type]}::const_iterator it = #{f[$name]}.begin(); it != #{f[$name]}.end(); ++it) {\n            this->#{vectorIds(f)}.push_back((*it)->getId());\n        }\n    }\n"
    #str << "    void #{cap(name)}::add#{cap(single(f[$name]))}(const #{getVectorGeneric(f[$type])}& #{single(f[$name])}) {\n        if (std::find(#{vectorIds(f)}.begin(), #{vectorIds(f)}.end(), #{single(f[$name])}Id) == #{vectorIds(f)}.end()) {\n                #{vectorIds(f)}.push_back(#{single(f[$name])}Id);\n                if (!#{f[$name]}.empty()) #{f[$name]}.push_back(#{getVectorGeneric(f[$type])}::findById(#{single(f[$name])}Id));\n        }\n    }\n"
    #str << "    void #{cap(name)}::remove#{cap(single(f[$name]))}(const #{getVectorGeneric(f[$type])}& #{single(f[$name])}) {\n        for (#{f[$type]}::iterator it = #{f[$name]}.begin(); it != #{f[$name]}.end(); ++it) {\n            if (#{single(f[$name])}Id == (*it)->getId()) {\n                delete (*it);\n                #{f[$name]}.erase(it);\n            }\n        }\n        for (vector<int>::iterator it = #{vectorIds(f)}.begin(); it != #{vectorIds(f)}.end(); ++it) {\n            if (#{single(f[$name])}Id == *it) {\n                #{vectorIds(f)}.erase(it);\n            }\n        }\n    }\n"
  elsif (isVector(f[$type]) && f[$attrib] & Attrib::ID > 0)
    objVec = plural(f[$name][0..-4])
    str << "    const #{f[$type]}& #{cap(name)}::get#{cap(f[$name])}() const { return #{f[$name]}; }\n"
    str << "    void #{cap(name)}::set#{cap(f[$name])}(const #{f[$type]}& #{f[$name]}) {\n        while (!#{objVec}.empty()) delete #{objVec}.back(), #{objVec}.pop_back();\n        this->#{f[$name]}.clear();\n        this->#{f[$name]} = #{f[$name]};\n    }\n"
    #str << "    void #{cap(name)}::add#{cap(single(f[$name]))}(const #{getVectorGeneric(f[$type])} #{single(f[$name])}) {\n        if (std::find(#{vectorIds(f)}.begin(), #{vectorIds(f)}.end(), #{single(f[$name])}Id) == #{vectorIds(f)}.end()) {\n                #{vectorIds(f)}.push_back(#{single(f[$name])}Id);\n                if (!#{f[$name]}.empty()) #{f[$name]}.push_back(#{getVectorGeneric(f[$type])}::findById(#{single(f[$name])}Id));\n        }\n    }\n"
    #str << "    void #{cap(name)}::remove#{cap(single(f[$name]))}(const #{getVectorGeneric(f[$type])} #{single(f[$name])}) {\n        for (#{f[$type]}::iterator it = #{f[$name]}.begin(); it != #{f[$name]}.end(); ++it) {\n            if (#{single(f[$name])}Id == (*it)->getId()) {\n                delete (*it);\n                #{f[$name]}.erase(it);\n            }\n        }\n        for (vector<int>::iterator it = #{vectorIds(f)}.begin(); it != #{vectorIds(f)}.end(); ++it) {\n            if (#{single(f[$name])}Id == *it) {\n                #{vectorIds(f)}.erase(it);\n            }\n        }\n    }\n"
  elsif (isSet(f[$type]))
    str << "    const #{f[$type]}& #{cap(name)}::get#{cap(f[$name])}() const {\n        return #{f[$name]};\n    }\n"
    str << "    void #{cap(name)}::set#{cap(f[$name])}(const #{f[$type]}& #{f[$name]}) {\n        this->#{f[$name]} = #{f[$name]};\n    }\n"
    str << "    void #{cap(name)}::add#{cap(single(f[$name]))}(const #{getSetGeneric(f[$type])}& #{single(f[$name])}) {\n        this->#{f[$name]}.insert(#{single(f[$name])});\n    }\n"
    str << "    void #{cap(name)}::remove#{cap(single(f[$name]))}(const #{getSetGeneric(f[$type])}& #{single(f[$name])}) {\n        this->#{f[$name]}.erase(#{single(f[$name])});\n    }\n"
  elsif (f[$type] == :int && f[$attrib] & Attrib::ID > 0)
    str << "    const #{f[$type]} #{cap(name)}::get#{cap(f[$name])}() const {\n        return (!#{f[$name]} && #{f[$name][0..-3]}) ? #{f[$name][0..-3]}->getId() : #{f[$name]};\n    }\n"
    str << "    void #{cap(name)}::set#{cap(f[$name])}(const #{f[$type]} #{f[$name]}) {\n        this->#{f[$name]} = #{f[$name]};\n        delete #{f[$name][0..-3]};\n        #{f[$name][0..-3]} = NULL;\n    }\n"
  else
    str << "    const #{f[$type]} #{cap(name)}::get#{cap(f[$name])}() const { return #{f[$name]}; }\n"
    str << "    void #{cap(name)}::set#{cap(f[$name])}(const #{f[$type]} #{f[$name]}) { this->#{f[$name]} = #{f[$name]}; }\n"
  end
  str << "\n"
end

######################### header file

def writeHeader (name, fields, attribs, customMethods, customHeaders)
  capName = cap(name)
  secondaryKeys = fields.select{|f| f[2] & Attrib::KEY2 > 0 }
  str = ""
  str << "//\n//  #{capName}.h\n//  soul-sifter\n//\n//  Created by Robby Neale\n//  Generated by generate_model.rb\n//\n\n#ifndef __soul_sifter__#{capName}__\n#define __soul_sifter__#{capName}__\n\n#include <set>\n#include <string>\n#include <vector>\n\n"
  str << customHeaders
  str << "#include \"ResultSetIterator.h\"\n"
  str << "\nnamespace sql {\n    class ResultSet;\n}\n\nusing namespace std;\n\nnamespace dogatech {\nnamespace soulsifter {\n\n"
  fields.transpose[0].uniq.each do |t|
    unless ([:int, :bool, :time_t, :string].include?(t))
      if (!isVector(t) && !isSet(t))
        str << "    class #{t};\n"
      elsif (isVector(t) && !["int", "string"].include?(getVectorGeneric(t)))
        str << "    class #{getVectorGeneric(t)};\n"
      elsif (isSet(t) && !["int", "string"].include?(getSetGeneric(t)))
        str << "    class #{getSetGeneric(t)};\n"
      end
    end
  end
  str << "\n    class #{capName} {\n    public:\n"
  str << hConstructor(name)
  str << hCopyConstructor(name)
  str << hAssignmentConstructor(name)
  str << hDestructor(name)
  str << hClearFunction()
  str << "\n"
  fields.select{|f| f[$attrib] & Attrib::FIND > 0}.each do |f|
    str << hFindFunction(name, f)
  end
  if (!secondaryKeys.empty?)
    str << hSecondaryKeysFindFunction(name, secondaryKeys)
  end
  str << hFindAllFunction(name)
  str << "\n"
  str << hUpdateFunction()
  str << hSaveFunction()
  str << hSyncFunction()
  if (attribs & Attrib::DELETABLE > 0)
    str << hEraseFunction()
  end
  str << "\n"
  str << customMethods
  fields.each do |f|
    str << hAccessor(f)
  end
  str << "\n        friend ResultSetIterator<#{cap(name)}>;\n"
  str << "        friend class SearchUtil;\n"
  str << "\n    private:\n"
  fields.each do |f|
    str << hFieldDeclaration(f)
  end
  str << "\n"
  str << hPopulateFieldFunctions(name, fields)
  str << "    };\n\n}\n}\n\n#endif /* defined(__soul_sifter__#{capName}__) */\n"
  return str
end

######################### c++ file

def writeCode (name, fields, attribs)
  capName = cap(name)
  secondaryKeys = fields.select{|f| f[$attrib] & Attrib::KEY2 > 0 }
  str = ""
  str << "//\n//  #{capName}.cpp\n//  soul-sifter\n//\n//  Created by Robby Neale\n//  Generated by generate_model.rb\n//\n\n#include \"#{capName}.h\"\n\n#include <cmath>\n#include <string>\n#include <sstream>\n\n#include <boost/regex.hpp>\n#include <boost/algorithm/string.hpp>\n#include <jdbc/cppconn/connection.h>\n#include <jdbc/cppconn/statement.h>\n#include <jdbc/cppconn/prepared_statement.h>\n#include <jdbc/cppconn/resultset.h>\n#include <jdbc/cppconn/exception.h>\n#include <jdbc/cppconn/warning.h>\n#include <g3log/g3log.hpp>\n\n#include \"MysqlAccess.h\"\n#include \"DTVectorUtil.h\"\n"
  fields.transpose[0].uniq.each do |t|
    unless ([:int, :bool, :time_t, :string].include?(t))
      if (!isVector(t) && !isSet(t))
        str << "#include \"#{t}.h\"\n"
      elsif (isVector(t) && !["int", "string"].include?(getVectorGeneric(t)))
        str << "#include \"#{getVectorGeneric(t)}.h\"\n"
      elsif (isSet(t) && !["int", "string"].include?(getSetGeneric(t)))
        str << "#include \"#{getSetGeneric(t)}.h\"\n"
      end
    end
  end
  str << "\nusing namespace std;\n\nnamespace dogatech {\nnamespace soulsifter {\n"
  str << "\n# pragma mark initialization\n\n"
  str << cConstructor(name, fields)
  str << cCopyConstructor(name, fields)
  str << cAssignmentConstructor(name, fields)
  str << cDestructor(name, fields)
  str << cClearFunction(name, fields)
  str << "# pragma mark static methods\n\n"
  str << cPopulateFieldFunctions(name, fields)
  fields.select{|f| f[$attrib] & Attrib::FIND > 0}.each do |f|
    str << cFindFunction(name, f, fields)
  end
  if (!secondaryKeys.empty?)
    str << cSecondaryKeysFindFunction(name, secondaryKeys, fields)
  end
  str << cFindAllFunction(name, fields)
  str << "\n# pragma mark persistence\n\n"
  str << cUpdateFunction(name, fields)
  str << cSaveFunction(name, fields, attribs)
  str << cSyncFunction(name, fields, secondaryKeys)
  if (attribs & Attrib::DELETABLE > 0)
    str << cEraseFunction(name, fields)
  end
  str << "\n# pragma mark accessors\n\n"
  fields.each do |f|
    str << cAccessor(name, f)
  end
  str << "}\n}\n"
end

######################### table definitions

albumPartFields = [
  [:int, "id", Attrib::FIND],
  [:string, "pos", Attrib::KEY2],
  [:string, "name", 0],
  [:int, "albumId", Attrib::ID | Attrib::KEY2],
  ["Album", "album", Attrib::PTR],
]
albumFields = [
  [:int, "id", Attrib::FIND],
  [:string, "name", Attrib::KEY2],
  [:string, "artist", Attrib::KEY2],
  [:string, "coverFilepath", Attrib::FIND],
  [:bool, "mixed", 0],
  [:string, "label", 0],
  [:string, "catalogId", 0],
  [:int, "releaseDateYear", 0],
  [:int, "releaseDateMonth", 0],
  [:int, "releaseDateDay", 0],
  [:int, "basicGenreId", Attrib::ID],
  ["BasicGenre", "basicGenre", Attrib::PTR],
]
albumCustomMethods = "        const string reReleaseDate() const;\n\n"
basicGenreFields = [
  [:int, "id", Attrib::FIND],
  [:string, "name", Attrib::KEY2],
]
basicGenreAttribs = 0
basicGenreCustomMethods = "        static BasicGenre* findByFilepath(const string& filepath);\n        static void findAll(const vector<const BasicGenre*>** genresPtr);\n        static BasicGenre* findByArtist(const string& artist);\n\n"
mixFields = [
  [:int, "id", Attrib::FIND],
  [:int, "outSongId", Attrib::ID | Attrib::KEY2],
  ["Song", "outSong", Attrib::PTR],
  [:int, "inSongId", Attrib::ID | Attrib::KEY2],
  ["Song", "inSong", Attrib::PTR],
  [:string, "bpmDiff", 0],
  [:int, "rating", 0],
  [:string, "comments", 0],
  [:bool, "addon", 0],
]
mixAttribs = 0
mixCustomMethods = "        friend class RapidEvolutionDatabaseMixoutsMixoutHandler;\n\n        static int mixoutCountForRESongId(int outSongId);\n\n"
musicVideoFields = [
  [:int, "id", Attrib::FIND],
  [:string, "filePath", 0],
  [:string, "thumbnailFilePath", 0],
]
musicVideoAttribs = 0
musicVideoCustomMethods = ""
playlistFields = [
  [:int, "id", Attrib::FIND],
  [:string, "name", Attrib::FIND],
  [:string, "query", 0],
  [:string, "gmusicId", 0],
  [:string, "youtubeId", 0],
  [:string, "spotifyId", Attrib::FIND],
  ["vector<int>", "styleIds", Attrib::ID],
  ["vector<Style*>", "styles", 0],
]
playlistAttribs = Attrib::DELETABLE
playlistCustomHeaders = ""
playlistEntryFields = [
  [:int, "id", Attrib::FIND],
  [:int, "playlistId", Attrib::ID | Attrib::KEY2],
  ["Playlist", "playlist", Attrib::PTR],
  [:int, "songId", Attrib::ID | Attrib::KEY2],
  ["Song", "song", Attrib::PTR],
  [:int, "position", Attrib::NON_NULLABLE],
]
playlistEntryAttribs = Attrib::DELETABLE
reSongFields = [
  [:int, "id", Attrib::FIND],
  [:string, "songidWinfo", 0],
  [:string, "songid", Attrib::FIND],
  [:string, "shortid", 0],
  [:string, "shortidWinfo", 0],
  [:string, "artist", 0],
  [:string, "album", 0],
  [:string, "track", 0],
  [:string, "title", 0],
  [:string, "time", 0],
  [:string, "timeSignature", 0],
  [:string, "filename", 0],
  [:string, "digitalOnly", 0],
  [:string, "compilation", 0],
  [:string, "keyStart", 0],
  [:int, "keyAccuracy", 0],
  [:string, "bpmStart", 0],
  [:int, "bpmAccuracy", 0],
  [:int, "rating", 0],
  [:string, "dateAdded", 0],
  [:string, "catalogId", 0],  # custom 4
  [:string, "label", 0],  # custom 3
  [:string, "remix", 0],
  [:int, "numPlays", 0],
  [:string, "comments", 0],
  [:string, "releaseDate", 0],  # custom 2
  [:string, "featuring", 0],  # custom 1
  [:string, "keyEnd", 0],
  [:string, "disabled", 0],
  [:string, "bpmEnd", 0],
  [:int, "beatIntensity", 0],
  [:string, "replayGain", 0],
  [:string, "stylesBitmask", Attrib::TRANSIENT],
]
reSongAttribs = Attrib::SAVEID
reSongCustomMethods = "        friend class RapidEvolutionDatabaseSongsSongHandler;\n\n        static const int maxREId();\n        void setStylesBitmaskFromDb();\n\n"
reSongCustomHeaders = ""
songFields = [
  [:int, "id", Attrib::FIND],
  [:string, "artist", 0],
  [:string, "track", 0],
  [:string, "title", 0],
  [:string, "remixer", 0],
  [:string, "featuring", 0],
  [:string, "filepath", Attrib::FIND],
  [:int, "rating", Attrib::NON_NULLABLE],
  [:time_t, "dateAdded", 0],
  [:string, "bpm", 0],
  [:string, "tonicKey", 0],
  [:int, "energy", 0],
  [:string, "comments", 0],
  [:bool, "trashed", 0],
  [:bool, "lowQuality", 0],
  [:string, "googleSongId", Attrib::FIND],
  [:string, "youtubeId", Attrib::FIND],
  [:string, "spotifyId", Attrib::FIND],
  [:int, "durationInMs", 0],
  [:string, "curator", 0],
  [:int, "reSongId", Attrib::KEY2 | Attrib::ID],
  ["RESong", "reSong", Attrib::PTR],
  [:int, "albumId", Attrib::ID],
  ["Album", "album", Attrib::PTR],
  [:int, "albumPartId", Attrib::ID],
  ["AlbumPart", "albumPart", Attrib::PTR],
  [:int, "musicVideoId", Attrib::ID],
  ["MusicVideo", "musicVideo", Attrib::PTR],
  ["vector<int>", "styleIds", Attrib::ID],
  ["vector<Style*>", "styles", 0],
  [:bool, "bpmLock", 0],
  [:bool, "tonicKeyLock", 0],
]
songAttribs = 0
songCustomMethods = "        explicit Song(RESong* song);\n\n        static RESong* createRESongFromSong(Song& song);\n\n        const string reAlbum();\n        const string getDateAddedString() const;\n        void setDateAddedToNow();\n\n"
songCustomHeaders = ""
styleFields = [
  [:int, "id", Attrib::FIND],
  [:string, "name", 0],
  [:int, "reId", Attrib::KEY2],
  [:string, "reLabel", 0],
  ["vector<int>", "childIds", Attrib::ID],
  ["vector<Style*>", "children", 0],
  ["vector<int>", "parentIds", Attrib::ID],
  ["vector<Style*>", "parents", 0],
]
styleAttribs = 0
styleCustomMethods = "        bool operator<(const Style& style) const;\n        bool operator>(const Style& style) const;\n\n        static ResultSetIterator<Style>* findAllParents();\n        static void findAllSortedByName(vector<Style*>** stylesPtr);\n        static void findAllSortedByREId(vector<Style*>** stylesPtr);\n\n"
styleCustomHeaders = ""
reSettingFields = [
  [:int, "id", Attrib::FIND],
  [:string, "name", Attrib::KEY2],
  [:string, "value", 0],
]
reSettingAttribs = 0
reSettingCustomMethods = "        static void findAll(vector<const RESetting*>* settings);\n\n        string& getValueRef();\n\n"
reSettingCustomHeaders = ""
reXmlFields = [
  [:int, "id", Attrib::FIND],
  [:string, "name", Attrib::KEY2],
  [:string, "xml", 0],
]
reXmlAttribs = 0
reXmlCustomMethods = "        string& getXmlRef();\n\n"
reXmlCustomHeaders = ""
reAlbumCoverFields = [
  [:int, "id", Attrib::FIND],
  [:string, "reId", Attrib::KEY2],
  [:string, "thumbnail", 0],
]
reAlbumCoverAttribs = 0
reAlbumCoverCustomMethods = ""
reAlbumCoverCustomHeaders = ""

######################### write files

output = File.open("src/Album.h", "w")
output << writeHeader("album", albumFields, 0, albumCustomMethods, "")
output.close
output = File.open("src/Album.cpp", "w")
output << writeCode("album", albumFields, 0)
output.close
output = File.open("src/AlbumPart.h", "w")
output << writeHeader("albumPart", albumPartFields, 0, "", "")
output.close
output = File.open("src/AlbumPart.cpp", "w")
output << writeCode("albumPart", albumPartFields, 0)
output.close
output = File.open("src/BasicGenre.h", "w")
output << writeHeader("basicGenre", basicGenreFields, basicGenreAttribs, basicGenreCustomMethods, "")
output.close
output = File.open("src/BasicGenre.cpp", "w")
output << writeCode("basicGenre", basicGenreFields, basicGenreAttribs)
output.close
output = File.open("src/Mix.h", "w")
output << writeHeader("mix", mixFields, mixAttribs, mixCustomMethods, "")
output.close
output = File.open("src/Mix.cpp", "w")
output << writeCode("mix", mixFields, mixAttribs)
output.close
output = File.open("src/MusicVideo.h", "w")
output << writeHeader("musicVideo", musicVideoFields, musicVideoAttribs, musicVideoCustomMethods, "")
output.close
output = File.open("src/MusicVideo.cpp", "w")
output << writeCode("musicVideo", musicVideoFields, musicVideoAttribs)
output.close
output = File.open("src/Playlist.h", "w")
output << writeHeader("playlist", playlistFields, playlistAttribs, "", playlistCustomHeaders)
output.close
output = File.open("src/Playlist.cpp", "w")
output << writeCode("playlist", playlistFields, playlistAttribs)
output.close
output = File.open("src/PlaylistEntry.h", "w")
output << writeHeader("playlistEntry", playlistEntryFields, playlistEntryAttribs, "", "")
output.close
output = File.open("src/PlaylistEntry.cpp", "w")
output << writeCode("playlistEntry", playlistEntryFields, playlistEntryAttribs)
output.close
output = File.open("src/RESong.h", "w")
output << writeHeader("reSong", reSongFields, reSongAttribs, reSongCustomMethods, reSongCustomHeaders)
output.close
output = File.open("src/RESong.cpp", "w")
output << writeCode("reSong", reSongFields, reSongAttribs)
output.close
output = File.open("src/Song.h", "w")
output << writeHeader("song", songFields, songAttribs, songCustomMethods, songCustomHeaders)
output.close
output = File.open("src/Song.cpp", "w")
output << writeCode("song", songFields, songAttribs)
output.close
output = File.open("src/Style.h", "w")
output << writeHeader("style", styleFields, styleAttribs, styleCustomMethods, styleCustomHeaders)
output.close
output = File.open("src/Style.cpp", "w")
output << writeCode("style", styleFields, styleAttribs)
output.close
output = File.open("src/RESetting.h", "w")
output << writeHeader("reSetting", reSettingFields, reSettingAttribs, reSettingCustomMethods, reSettingCustomHeaders)
output.close
output = File.open("src/RESetting.cpp", "w")
output << writeCode("reSetting", reSettingFields, reSettingAttribs)
output.close
output = File.open("src/REXML.h", "w")
output << writeHeader("reXml", reXmlFields, reXmlAttribs, reXmlCustomMethods, reXmlCustomHeaders)
output.close
output = File.open("src/REXML.cpp", "w")
output << writeCode("reXml", reXmlFields, reXmlAttribs)
output.close
output = File.open("src/REAlbumCover.h", "w")
output << writeHeader("reAlbumCover", reAlbumCoverFields, reAlbumCoverAttribs, reAlbumCoverCustomMethods, reAlbumCoverCustomHeaders)
output.close
output = File.open("src/REAlbumCover.cpp", "w")
output << writeCode("reAlbumCover", reAlbumCoverFields, reAlbumCoverAttribs)
output.close
