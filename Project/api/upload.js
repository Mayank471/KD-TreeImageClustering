const fs = require('fs');
const path = require('path');

export default async function handler(req, res) {
  // Enable CORS
  res.setHeader('Access-Control-Allow-Origin', '*');
  res.setHeader('Access-Control-Allow-Methods', 'POST, OPTIONS');
  res.setHeader('Access-Control-Allow-Headers', 'Content-Type');
  
  if (req.method === 'OPTIONS') {
    return res.status(200).end();
  }

  if (req.method !== 'POST') {
    return res.status(405).json({ success: false, message: 'Method not allowed' });
  }

  try {
    // Check if we can access the body
    console.log('Upload - Request content-type:', req.headers['content-type']);
    console.log('Upload - Request method:', req.method);
    
    // Simple check for FormData (multipart/form-data will be present)
    if (!req.headers['content-type'] || !req.headers['content-type'].includes('multipart/form-data')) {
      return res.status(400).json({ 
        success: false, 
        message: 'Invalid content type. Expected multipart/form-data.' 
      });
    }

    // For now, we'll just acknowledge the upload
    // In a full implementation, we'd process the image here
    return res.json({ 
      success: true, 
      message: 'Image uploaded successfully',
      filename: 'uploaded_image.jpg'
    });
    
  } catch (error) {
    console.error('Upload error:', error);
    console.error('Upload error stack:', error.stack);
    return res.status(500).json({ success: false, message: 'Upload failed: ' + error.message });
  }
}