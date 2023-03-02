import { defineConfig } from 'vite';

// https://vitejs.dev/config/
export default defineConfig({
  base: process.env.IS_DEV !== 'true' ? './' : '/',
  build: {
    minify: false,
    outDir: 'build',
  },
  esbuild: {
    keepNames: true,
  },
  plugins: [],
});