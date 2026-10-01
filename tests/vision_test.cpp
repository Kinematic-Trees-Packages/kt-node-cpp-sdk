#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

#include <ktnode/vision.hpp>
#include <kt/messages/vision_sample_generated.h>

int main() {
  ktnode::vision::ImageFrame frame;
  frame.source = "unit-test";
  frame.width = 2;
  frame.height = 1;
  frame.channels = 3;
  frame.frame_number = 7;
  frame.captured_unix_ns = 42;
  frame.data = {1, 2, 3, 4, 5, 6};

  auto payload = ktnode::vision::encode_image_sample(frame);
  assert(payload.size() > frame.data.size());
  auto* sample = kt::messages::vision_sample::GetImageSample(payload.data());
  assert(sample->source()->str() == "unit-test");
  assert(sample->frame_number() == 7);
  assert(sample->captured_unix_ns() == 42);
  assert(sample->data_shape()->size() == 3);
  assert(sample->data_shape()->Get(0) == 1);
  assert(sample->data_shape()->Get(1) == 2);
  assert(sample->data_shape()->Get(2) == 3);
  assert(sample->compression() == kt::messages::vision_sample::CompressionFormat::RAW);
  assert(sample->image_type() == kt::messages::vision_sample::ImageType::RGB);
  assert(sample->data()->size() == 6);
  std::cout << "cpp vision test passed\n";
}
