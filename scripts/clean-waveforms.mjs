#!/usr/bin/env node
import fs from 'node:fs';
import path from 'node:path';
import os from 'node:os';

const AUDIO_EXTENSIONS = ['.mp3', '.m4a', '.mp4', '.flac', '.wav', '.aiff', '.aif', '.aac', '.alac', '.ogg', '.opus', '.wma'];

// Load settings from SoulSifter settings.json or env/args
function resolveDirectories() {
  let musicDir = process.env.MUSIC_DIR;
  let waveformsDir = process.env.WAVEFORMS_DIR;

  const settingsPath = path.join(os.homedir(), 'Library', 'Application Support', 'SoulSifter', 'settings.json');
  if ((!musicDir || !waveformsDir) && fs.existsSync(settingsPath)) {
    try {
      const data = JSON.parse(fs.readFileSync(settingsPath, 'utf8'));
      musicDir = musicDir || data.dir?.music || data['dir.music'];
      waveformsDir = waveformsDir || data.dir?.waveforms || data['dir.waveforms'];
    } catch (err) {
      console.warn('Could not parse SoulSifter settings.json:', err.message);
    }
  }

  return { musicDir, waveformsDir };
}

function findFiles(dir, ext = '.webp') {
  let results = [];
  if (!fs.existsSync(dir)) return results;
  const list = fs.readdirSync(dir, { withFileTypes: true });
  for (const dirent of list) {
    const fullPath = path.join(dir, dirent.name);
    if (dirent.isDirectory()) {
      results = results.concat(findFiles(fullPath, ext));
    } else if (dirent.name.endsWith(ext)) {
      results.push(fullPath);
    }
  }
  return results;
}

function cleanEmptyDirs(dir) {
  if (!fs.existsSync(dir)) return;
  for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
    if (entry.isDirectory()) {
      const subDir = path.join(dir, entry.name);
      cleanEmptyDirs(subDir);
      if (fs.readdirSync(subDir).length === 0) {
        fs.rmdirSync(subDir);
      }
    }
  }
}

async function main() {
  const isDelete = process.argv.includes('--delete') || process.argv.includes('-d');
  const { musicDir, waveformsDir } = resolveDirectories();

  if (!musicDir || !waveformsDir) {
    console.error('Error: Could not locate dir.music or dir.waveforms. Please set MUSIC_DIR and WAVEFORMS_DIR environment variables or configure them in SoulSifter.');
    process.exit(1);
  }

  console.log(`Music Dir:     ${musicDir}`);
  console.log(`Waveforms Dir: ${waveformsDir}`);
  console.log(`Mode:          ${isDelete ? 'DELETE' : 'DRY RUN (preview only)'}\n`);

  const waveformFiles = findFiles(waveformsDir, '.webp');
  const orphaned = [];

  for (const wfPath of waveformFiles) {
    const relPath = path.relative(waveformsDir, wfPath);
    // Strip dimension suffix (_24x200.webp or .webp)
    const baseRelPath = relPath.replace(/(_\d+x\d+)?\.webp$/, '');

    const hasMatchingMusic = AUDIO_EXTENSIONS.some(ext =>
      fs.existsSync(path.join(musicDir, `${baseRelPath}${ext}`))
    );

    if (!hasMatchingMusic) {
      orphaned.push(wfPath);
    }
  }

  if (orphaned.length === 0) {
    console.log('No orphaned waveform images found.');
    return;
  }

  console.log(`Found ${orphaned.length} unused waveform image(s):`);
  for (const file of orphaned) {
    if (isDelete) {
      fs.unlinkSync(file);
      console.log(`Deleted: ${file}`);
    } else {
      console.log(`Unused:  ${file}`);
    }
  }

  if (isDelete) {
    cleanEmptyDirs(waveformsDir);
    console.log(`\nSuccessfully removed ${orphaned.length} unused waveform(s).`);
  } else {
    console.log(`\nRun with --delete (or 'npm run clean:waveforms') to remove them.`);
  }
}

main();
