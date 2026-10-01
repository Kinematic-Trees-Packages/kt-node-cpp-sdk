#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <flatbuffers/flatbuffers.h>
#include <kt/messages/vision_sample_generated.h>

namespace ktnode::vision {

namespace messages = kt::messages::vision_sample;

struct ImageFrame {
  std::string source;
  uint32_t width = 0;
  uint32_t height = 0;
  uint32_t channels = 0;
  uint64_t frame_number = 0;
  uint64_t captured_unix_ns = 0;
  std::vector<uint8_t> data;
};

inline std::vector<uint8_t> encode_image_sample(const ImageFrame& frame) {
  if (frame.source.empty()) throw std::invalid_argument("ImageFrame.source is required");
  if (frame.width == 0 || frame.height == 0 || frame.channels == 0) {
    throw std::invalid_argument("ImageFrame shape must be non-zero");
  }
  const size_t expected = static_cast<size_t>(frame.width) * frame.height * frame.channels;
  if (frame.data.size() != expected) {
    throw std::invalid_argument("ImageFrame.data length does not match width*height*channels");
  }

  flatbuffers::FlatBufferBuilder builder(frame.data.size() + 256);
  auto source = builder.CreateString(frame.source);
  auto data = builder.CreateVector(frame.data);
  uint32_t shape_values[3] = {frame.height, frame.width, frame.channels};
  auto shape = builder.CreateVector(shape_values, 3);
  auto sample = messages::CreateImageSample(
      builder,
      source,
      data,
      shape,
      messages::CompressionFormat::RAW,
      messages::ImageType::RGB,
      0,
      frame.frame_number,
      messages::StereoDesignation::NONE,
      0.0f,
      0.0f,
      true,
      0,
      0,
      messages::MediaPipeline::OTHER,
      0,
      frame.captured_unix_ns,
      messages::DepthRepresentation::UNSPECIFIED,
      messages::DepthColorization::NONE,
      0.0f);
  builder.Finish(sample, "VSM1");
  const uint8_t* begin = builder.GetBufferPointer();
  return std::vector<uint8_t>(begin, begin + builder.GetSize());
}

}  // namespace ktnode::vision
