Soul Sifter
===========

DJ & music organization app.

This is created by me for me.

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
  * run restore_db.sh to create all the tables (the last line to populate the data may fail, but that's okay if you don't have any)
  * git_add_update.sh script available if you periodically wish to take snapshots of your data and save it to a git repo. Can be really handy to take a backup using it before running any scripts on the db, and then git diff allows for a good comparison to make sure changes are expected.
* (to build) install npm modules
  * global: node-gyp
  * locally with npm install
* required libs (brew install): mysql-connector-c++ (-DWITH_JDBC=ON), libtag, mad, boost, youtube-dl (for music videos), ffmpeg, g3log

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

I originally housed my music in [Rapid Evolution](https://en.wikipedia.org/wiki/Rapid_Evolution). It was a sweet music app which work eventually ceased on.

The mentions of RE are from where I previously housed my music - Rapid Evolution. It was an amazing music app which organized, analyzed key & bpm, and synced with songkick iirc. Development ceased on it though, and my music db was stuck inside and I wanted more control over the data.

Thus, I created the first version of [SoulSifter](https://github.com/broken/soul-sifter) on Aug 17, 2012. There was a lot of syncing back & forth between the RE db & mine early on, but my philosophy was to not lock myself (or anybody else) in. All data was stored in MySQL so if something better came along you could write a script and move the data over. It was painful to build the UI using Xcode and Objective-C.

When I found out about Node-Webkit (later NW.js) and discovered you could write a desktop app using web APIs I was immidiately sold. The new [SoulSifter v0.0](https://github.com/broken/soulsifter) repository was just shy of two years later on Aug 13, 2014. The app flourished, features were added more quickly, the GUI looked much better, and most of the backend code was the same - compiling the C++ into native js components. However, it had its own pain points. The main one being it did not support drag & drop (which was required for DJing), so any upgrade the NW.js code would need to be hacked and recompiled which would take a full day. At one point I was having to compile in ffmpeg codec myself. I felt nw-gyp was the janky and not-often-looked at cousin of node-gyp. There was simply a lot of time wasted getting everything working that should have been given to feature development.

In Feb of 2023, at v0.18, I upgraded my laptop and libraries got out of wack to the point I was no longer able to build a working copy of SoulSifter. I was fed up with NW.js, and the promise of native file drag & drop Electron delivered inticed me to switch. In parallel, it seemed converting over to the new Electron framework would be faster then getting the current version working. It had been something I had wanted to do for some time, which had me limiting any new feature development anyways, and this was the forcing function. Now the third incarnation, [SoulSifter v1.0]() was born Feb 27, 2023. I've been working on SoulSifter in one form or another for over a decade, and I plan to work on it for decades to come.
