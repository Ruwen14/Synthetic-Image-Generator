# simG (Synthetic Image Generation)
 ImageGenerator C++

# Requirements
- CMake
- OpenCV-4.5.0+
- **(Already included!)** Json-Library

 
# Demo
```cpp
#include "ImageGenerator2D.h"
#include "Utils/Directory.h"
#include <iostream>

// Modules are wrapped in namespace simG
int main() 
{
```
#### Set Settings - change degree of Augmentations
```cpp
   simG::ImageGenerator2D::AugmentationParams params;
   
   // Defaults are equivalent to ↓↓↓. Change to your liking.
   params.randomnoise = false;
   params.BackgroundAugs.axisflipping.do_random_flip = true;
   params.BackgroundAugs.axisflipping.include_no_flip = true;
   params.BackgroundAugs.brightness.do_shift = true;
   params.BackgroundAugs.brightness.brightness_range = { -20.5, 20.5 };
   params.BackgroundAugs.crop.do_crop = true;
   params.BackgroundAugs.crop.target_dim = { 1024, 576 };
   params.BackgroundAugs.crop.keep_aspect = true;
   params.MaskAugs.rotation.do_rotate = true;
   params.MaskAugs.rotation.rotation_range = { 0, 360 };
   params.MaskAugs.brightness.do_shift = true;
   params.MaskAugs.brightness.brightness_range = { -20.5, 20.5 };
   // ↑↑↑
```
#### Create Generator
```cpp
  simG::Directory maskDir("./transportation/car");
  simg::Directory backgroundDir("./backgrounds");
  int numberImagesToGenerate = 500;
  int maxObjectsPerImage = 5;
  simG::ImageGenerator2D generator(maskDir, 
                                   backgroundDir, 
                                   numberImagesToGenerate, 
                                   maxObjectsPerImage, 
                                   params);
```
#### (Optional) Add Annotator 
```cpp
  simG::CocoAnnotator annotator("path-to-annotationfile");
  // or 
  simG::YoloAnnotator annotator("path-to-annotationfile");
  // or ...
  simG::ImageGenerator2D generator(maskDir, 
                                   backgroundDir, 
                                   numberImagesToGenerate, 
                                   maxObjectsPerImage, 
                                   params,
                                   &annotator); // ← pass annotator to generator
```

#### Generate Images
- **Single Step:**  *Advances the whole generator and yields one image. Recommended for debugging or custom purposes.*
```cpp
  auto result_img = generator.forward();
  // do custom stuff with yielded image
```
- **Create Dataset:** *Creates images in a loop and saves them according to the previous constructed arguments.*
```cpp
  generator.forwardloop();
  // generates a whole dataset according to numberImagesToGenerate
```
- **Multithreading:** *add multiple worker-threads who read, process and save the data in parallel for faster results.*
```cpp
generator.setThreading(simG::ThreadingStatus::NUM_THREADS_4);
generator.forwardloop();
``` 

# Results

