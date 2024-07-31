Soul Sifter
===========

DJ & music organization app.

This is created for a user base of one - me.

It would be awesome if others found it useful, but I'm focused on features I need versus making it available for others at this time.

Fresh Build Instructions
------------------------
Presteps:
* create mysql db
  * SELECT @@GLOBAL.sql_mode; SET GLOBAL sql_mode=(SELECT REPLACE(@@sql_mode,'ONLY_FULL_GROUP_BY',''));
  * update /etc/mysql/my.cnf to with the new sql-mode= under [mysqld]
    * example: [mysqld]
      secure_file_priv=/Users/dogatech/Music/db
      sql-mode="STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION"
  * create music database with user ss, password pw, read & write creds to music db.
    * create database music;
    * create user 'ss'@'localhost' identified by 'pw';
    * grant all privileges on music.* to 'ss'@'localhost';
    * create user 'ssadmin'@'localhost' identified by 'pw';
    * grant all privileges on music.* to 'ssadmin'@'localhost';
    * grant SESSION_VARIABLES_ADMIN on *.* to 'ssadmin'@'localhost';
    * grant FILE on *.* to 'ssadmin'@'localhost';
    * grant PROCESS on *.* to 'ssadmin'@'localhost';
    * SET GLOBAL local_infile=1;
    * flush privileges;
  * run restore_db.sh to create all the tables (the last line to populate the data may fail, but that's okay if you don't have any)
  * git_add_update.sh script available if you periodically wish to take snapshots of your data and save it to a git repo. Can be really handy to take a backup using it before running any scripts on the db, and then git diff allows for a good comparison to make sure changes are expected.
* (to build) install npm modules
  * global: node-gyp
  * locally with npm install
* required libs (brew install): mysql-connector-c++ (-DWITH_JDBC=ON; brew install --build-from-source ./mysql-connector-c++.rb), libtag, mad, boost, ffmpeg, g3log (1.3.4 (<=c++14)), cliclick
  * sudo launchctl config user path /usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin
* It can read MixedInKey values from id3 tags. Settings:
  * Update custom "initial key" tag
  * Write energy tag in front of the grouping
  * Key notation: flats (do others work?)

```
npm run all
```

Music Database
--------------
The music database is stored in mysql. I chose this because of simplicity. Writing scripts & functions against mysql in multiple languages is well documented and straight forward. I can easily install it on any OS and manipulate the data. I also like the visualization provided by the mysql workbench and being able to generate the schemas from it.

Model Code Generation
---------------------
When I first started, I found myself doing much of the same work for the model objects. Rather than creating a hard to follow template, I created a ruby script to generate the model objects for me. This has proved to make model edits easy and safe.

On the same token, the wrappers for node-addon-api are also generated. However, this code utilizes antlr and is created in a separate project [napi-wrapper-gen](https://github.com/broken/napi-wrapper-gen). I did just enough to get what I needed, but again it makes edits quick, safe, and painless.


A brief history
===============

I originally housed my music in [Rapid Evolution](https://en.wikipedia.org/wiki/Rapid_Evolution). It was a great music app which organized, analyzed key & bpm, and synced with songkick iirc. Development ceased on it though, and my music db was stuck inside and I wanted control over that data.

Thus, I created the first version of [SoulSifter](https://github.com/broken/soul-sifter) on Aug 17, 2012. There was a lot of syncing back & forth between the RE db & mine early on, since it lacked many of the features, but my philosophy was to not lock myself (or anybody else) in. All data was stored in MySQL so if something better came along you could write a script and move the data over. The UI was using Xcode and Objective-C, and it was painful to write in.

When I found out about Node-Webkit (later NW.js) and discovered you could write a desktop app using web APIs I was immidiately sold. The new [SoulSifter v0.0](https://github.com/broken/soulsifter) repository was founded just shy of two years later on Aug 13, 2014. The app flourished, features were added more quickly, the GUI looked much better, and most of the backend code was the same - compiling the backend C++ into native js components. However, it had its own pain points. The main one being it did not support drag & drop (which was required for DJing), so any upgrade to NW.js would need to be hacked and recompiled which would take a full day. At one point I was having to compile in the ffmpeg codec myself. It relied on nw-gyp, which was the janky and not-often-looked at cousin of node-gyp. There was simply a lot of time wasted getting everything working that should have been devoted to feature development.

On Feb of 2023, at v0.18, I upgraded my laptop and the libraries got out of wack to the point I was no longer able to build a working copy of SoulSifter. I was fed up with NW.js, and the promise of native file drag & drop that Electron delivered inticed me to switch. Plus, it seemed converting over to the new Electron framework would be faster then getting the current version working. It had been something I had wanted to do for some time, which had me limiting any new feature development anyways, and this was the forcing function. Now the third incarnation, [SoulSifter v1.0](https://github.com/broken/soulsifter-e) was born Feb 27, 2023, and I've been much happier on this new framework. Looking back, I've been working on SoulSifter in one form or another for over a decade, and I plan to work on it for decades to come.


Release process
===============
* npm run release
* create release on github. tag version. upload zipped build.
* locally move app from out to applications
* pull new tag
* if minor release, do below, otherwise if patch release, do: npm run bump:patch && git push
* npm run branch
* git push -u origin <new_branch> && git checkout master
* npm run bump:minor
