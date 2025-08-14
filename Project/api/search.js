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
    console.log('Request content-type:', req.headers['content-type']);
    console.log('Request method:', req.method);
    
    // Simple check for FormData (multipart/form-data will be present)
    if (!req.headers['content-type'] || !req.headers['content-type'].includes('multipart/form-data')) {
      return res.status(400).json({ 
        success: false, 
        message: 'Invalid content type. Expected multipart/form-data.' 
      });
    }

    // Since this is a demo API with hardcoded results, we don't need to actually parse the file
    // We can just check for the presence of form data and return mock results
    
    // For demo purposes, return some actual image filenames from the Images directory
    const sampleImageNames = [
      '102.jpg', '177.jpg', '178.jpg', '179.jpg', '180.jpg',
      '181.jpg', '182.jpg', '183.jpg', '184.jpg', '185.jpg',
      '186.jpg', '187.jpg', '188.jpg', '189.jpg', '190.jpg'
    ];
    
    // Default to 5 results for demo
    const numberOfResults = 5;
    
    // Shuffle and pick random images for demo
    const shuffled = sampleImageNames.sort(() => 0.5 - Math.random());
    const results = shuffled.slice(0, Math.min(numberOfResults, shuffled.length));
    
    const mockResults = {
      success: true,
      message: 'Search complete',
      target: 'uploaded_image.jpg',
      num: numberOfResults,
      numbers: results
    };
    
    console.log('Returning results:', mockResults);
    return res.json(mockResults);
    
  } catch (error) {
    console.error('Search error:', error);
    console.error('Error stack:', error.stack);
    return res.status(500).json({ 
      success: false, 
      message: 'An error occurred during processing: ' + error.message 
    });
  }
}