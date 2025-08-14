const formidable = require('formidable');
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
    const form = formidable({
      uploadDir: '/tmp',
      keepExtensions: true,
      maxFileSize: 10 * 1024 * 1024, // 10MB limit
    });

    const [fields, files] = await form.parse(req);
    
    if (!files.image) {
      return res.status(400).json({ success: false, message: 'No file uploaded' });
    }

    const uploadedFile = Array.isArray(files.image) ? files.image[0] : files.image;
    
    // For now, we'll just acknowledge the upload
    // In a full implementation, we'd process the image here
    return res.json({ 
      success: true, 
      message: 'Image uploaded successfully',
      filename: uploadedFile.originalFilename
    });
    
  } catch (error) {
    console.error('Upload error:', error);
    return res.status(500).json({ success: false, message: 'Upload failed' });
  }
}