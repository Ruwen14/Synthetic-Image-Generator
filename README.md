# Synthetic-Image-Generator
 ImageGenerator C++
 
# Demo
```cpp
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
#### Add Generator
```cpp
  simG::Directory maskDir("./transportation/car");
  simg::Directory backgroundDir("./backgrounds");
  int numberImagesToGenerate = 500;
  int maxObjectsPerImage = 5;
  simG::ImageGenerator2D generator(maskDir, backgroundDir, numberImagesToGenerate, maxObjectsPerImage, params);
```
#### (Optional) Add Annotator 
```cpp
  simG::CocoAnnotator annotator("path-to-annotationfile");
  // or 
  simG::YoloAnnotator annotator("path-to-annotationfile");
  // or ...
  //                                                                                                        add to constructor
  //                                                                                                               ↓↓↓
  simG::ImageGenerator2D generator(maskDir, backgroundDir, numberImagesToGenerate, maxObjectsPerImage, params, &annotator);
```
#### Generate Images
```cpp
 
  if(!ImgaGenerator.isValid())
  {
    exit(1);
  }
  
  while(!generator.hasFinished())
  {
    generator.generate();
  }
}
```
