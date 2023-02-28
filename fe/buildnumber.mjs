import { readFile, writeFile } from 'fs';

console.log('Incrementing build number...');
var filename = 'package.json';
readFile(filename, function(err,content) {
  if (err) throw err;
  var metadata = JSON.parse(content);
  metadata.build = metadata.build + 1;
  writeFile('package.json', JSON.stringify(metadata, null, 2), function(err) {
    if (err) throw err;
    console.log(`Current version: ${metadata.version} (build ${metadata.build})`);
  });
});

