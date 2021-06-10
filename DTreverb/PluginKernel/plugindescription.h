// --- CMAKE generated variables for your plugin

#include "pluginstructures.h"

#ifndef _plugindescription_h
#define _plugindescription_h

#define QUOTE(name) #name
#define STR(macro) QUOTE(macro)
#define AU_COCOA_VIEWFACTORY_STRING STR(AU_COCOA_VIEWFACTORY_NAME)
#define AU_COCOA_VIEW_STRING STR(AU_COCOA_VIEW_NAME)

// --- AU Plugin Cocoa View Names (flat namespace) 
#define AU_COCOA_VIEWFACTORY_NAME AUCocoaViewFactory_E5764A39D21A3ED4A52EB5123851A439
#define AU_COCOA_VIEW_NAME AUCocoaView_E5764A39D21A3ED4A52EB5123851A439

// --- BUNDLE IDs (MacOS Only) 
const char* kAAXBundleID = "developer.aax.dtreverb.bundleID";
const char* kAUBundleID = "developer.au.dtreverb.bundleID";
const char* kVST3BundleID = "developer.vst3.dtreverb.bundleID";

// --- Plugin Names 
const char* kPluginName = "DTreverb";
const char* kShortPluginName = "DTreverb";
const char* kAUBundleName = "DTreverb";

// --- Plugin Type 
const pluginType kPluginType = pluginType::kFXPlugin;

// --- VST3 UUID 
const char* kVSTFUID = "{e5764a39-d21a-3ed4-a52e-b5123851a439}";

// --- 4-char codes 
const int32_t kFourCharCode = 'dtrv';
const int32_t kAAXProductID = 'dtrv';
const int32_t kManufacturerID = 'MOON';

// --- Vendor information 
const char* kVendorName = "MOON";
const char* kVendorURL = "";
const char* kVendorEmail = "moonsh3222@gmail.com";

// --- Plugin Options 
const bool kWantSidechain = false;
const uint32_t kLatencyInSamples = 0;
const double kTailTimeMsec = 0.000000;
const bool kVSTInfiniteTail = false;
const bool kVSTSAA = false;
const uint32_t kVST3SAAGranularity = 1;
const uint32_t kAAXCategory = 8;

#endif
