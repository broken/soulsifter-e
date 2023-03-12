# script which restores the database to the current dump files

echo "Please enter mysql root password (not saved):"
read pw

echo 'drop table if exists PlaylistStyles; drop table if exists PlaylistEntries; drop table if exists Playlists; drop table if exists SongStyles; drop table if exists Mixes; drop table if exists Songs; drop table if exists MusicVideos; drop table if exists StyleChildren; drop table if exists Styles; drop table if exists RESongs; drop table if exists AlbumParts; drop table if exists Albums; drop table if exists BasicGenres; drop table if exists REAlbumCovers; drop table if exists REXml; drop table if exists RESettings;' | mysql -uroot -p$pw music

mysql -uroot -p$pw music < RESettings.sql
mysql -uroot -p$pw music < REXml.sql
mysql -uroot -p$pw music < REAlbumCovers.sql
mysql -uroot -p$pw music < BasicGenres.sql
mysql -uroot -p$pw music < Albums.sql
mysql -uroot -p$pw music < AlbumParts.sql
mysql -uroot -p$pw music < RESongs.sql
mysql -uroot -p$pw music < Styles.sql
mysql -uroot -p$pw music < StyleChildren.sql
mysql -uroot -p$pw music < MusicVideos.sql
mysql -uroot -p$pw music < Songs.sql
mysql -uroot -p$pw music < Mixes.sql
mysql -uroot -p$pw music < SongStyles.sql
mysql -uroot -p$pw music < Playlists.sql
mysql -uroot -p$pw music < PlaylistEntries.sql
mysql -uroot -p$pw music < PlaylistStyles.sql

mysqlimport --local -uroot -p$pw music RESettings.txt REXml.txt REAlbumCovers.txt BasicGenres.txt Albums.txt AlbumParts.txt RESongs.txt Styles.txt StyleChildren.txt MusicVideos.txt Songs.txt Mixes.txt SongStyles.txt Playlists.txt PlaylistEntries.txt PlaylistStyles.txt
