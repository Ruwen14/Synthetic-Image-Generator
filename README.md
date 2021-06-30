# Synthetic-Image-Generator
 ImageGenerator C++
 
# Demo

```cpp
int main() {
  simG::ImageGenerator2D::AugmentationParams params;
  // Defaults
  //params.randomnoise = false;
  
  //params.BackgroundAugs.axisflipping.do_random_flip = true;
  //params.BackgroundAugs.axisflipping.include_no_flip = true;
  //params.BackgroundAugs.brightness.do_shift = true;
  //params.BackgroundAugs.brightness.brightness_range = { -20.5, 20.5 };
  //params.BackgroundAugs.crop.do_crop = true;
  //params.BackgroundAugs.crop.target_dim = { 1024, 576 };
  //params.BackgroundAugs.crop.keep_aspect = true;

  //params.MaskAugs.rotation.do_rotate = true;
  //params.MaskAugs.rotation.rotation_range = { 0, 360 };
  //params.MaskAugs.brightness.do_shift = true;
  //params.MaskAugs.brightness.brightness_range = { -20.5, 20.5 };
  
  simG::ImageGenerator2D generator(R(C:\Users\ruwen\Desktop\SyntheticDataGenerator_Bachelor\Dataset\input\templates\transportation\car)",
		R"(C:\Users\ruwen\Desktop\Learning_CPP\Synthethic-Image-Generator\Test)", 500, 5, params);

  if (!ImgaGenerator.is_generator_valid())
  {
    exit(1);
  }
 
  while (!generator.hasFinished())
  {
    generator.generate();
  }


}
```
