# KDTreeImageClustering - Vercel Deployment

This project has been configured for deployment on Vercel. 

## Deployment Instructions

1. **Fork or clone this repository**
2. **Connect to Vercel:**
   - Go to [vercel.com](https://vercel.com)
   - Sign up/log in with your GitHub account
   - Click "New Project"
   - Import this repository

3. **Deploy:**
   - Vercel will automatically detect the configuration from `vercel.json`
   - Click "Deploy"
   - Your app will be available at a Vercel URL

## What was changed for Vercel

The original project used C++ executables and Express.js server which don't work in Vercel's serverless environment. Changes made:

1. **Added Vercel configuration** (`vercel.json`)
2. **Created serverless API functions** in `Project/api/`:
   - `upload.js` - handles image uploads
   - `search.js` - handles image similarity search (simplified demo version)
3. **Updated frontend** to use relative API paths instead of localhost
4. **Added formidable dependency** for handling file uploads in serverless functions

## Current Limitations

The current deployment is a **simplified demo version** because:

- The original C++ image processing (`histogram.exe`, `project.exe`) cannot run on Vercel
- The KD-Tree clustering algorithm needs to be reimplemented in JavaScript
- Image similarity search currently returns random sample images

## For Full Functionality

To restore the full image clustering functionality, you would need to:

1. **Implement image histogram calculation in JavaScript** using libraries like:
   - `jimp` for image processing
   - `sharp` for high-performance image processing

2. **Implement KD-Tree clustering in JavaScript**

3. **Add persistent storage** for image data (since Vercel's filesystem is read-only)

4. **Optimize for serverless constraints** (execution time limits, memory limits)

## Usage

Once deployed:
1. Visit your Vercel app URL
2. Upload images using the upload form
3. Search for similar images using the search form
4. Results will display below (currently shows sample images)

## Local Development

To run locally for development:
```bash
cd Project
npm install
npm start
```

The local version still uses the original C++ executables if available.