//
//  SoulSifterSettings.h
//  soul-sifter
//
//  Created by Robby Neale on 12/9/12.
//
//

#ifndef __soul_sifter__SoulSifterSettings__
#define __soul_sifter__SoulSifterSettings__

#include <string>

#include <boost/property_tree/ptree.hpp>

using namespace std;

namespace dogatech {
namespace soulsifter {

    class SoulSifterSettings {
    public:
        static SoulSifterSettings& getInstance() {
            static SoulSifterSettings instance;  // guaranteed to be destroyed
            // instantiated on first use
            return instance;
        }

        void save();

        template <typename T>
        T get(const std::string& label) const;

        template <typename T>
        void put(const std::string& label, const T& value);

        string getString(const string& label) const;
        void putString(const string& label, const std::string& value);
        int getInt(const string& label) const;
        void putInt(const string& label, const int value);
        bool getBool(const string& label) const;
        void putBool(const string& label, const bool value);
    private:
        // singleton
        SoulSifterSettings();
        SoulSifterSettings(SoulSifterSettings const&);
        void operator=(SoulSifterSettings const&);

        boost::property_tree::ptree ptree;

        std::string filename;
    };

}
}

#endif /* defined(__soul_sifter__SoulSifterSettings__) */
