#pragma once
#include <Engine/Node/SimpleApi.hpp>

#include <cmath>
namespace MyAudioEffect
{
struct Distortion
{
  /**
   * An audio effect plug-in must provide some metadata: name, author, etc.
   */
  struct Metadata : Control::Meta_base
  {
    static const constexpr auto prettyName = "Distortion";
    static const constexpr auto objectKey = "Distortion";
    static const constexpr auto category = "Audio";
    static const constexpr auto author = "<AUTHOR>";
    static const constexpr auto kind = Process::ProcessCategory::AudioEffect;
    static const constexpr auto description = "<DESCRIPTION>";
    static const constexpr auto tags = std::array<const char*, 0>{};
    static const uuid_constexpr auto uuid
        = make_uuid("00000000-0000-0000-0000-000000000000");

    /**
     * Here we define the inputs & outputs ports of our plug-in.
     */
    static const constexpr auto controls
        = std::make_tuple(Control::FloatSlider{"Gain", 0., 2., 1.});
    static const constexpr audio_in audio_ins[]{"in"};
    static const constexpr audio_out audio_outs[]{"out"};
  };

  /**
   * This is used to define the sample-accuracy level of this plug-in ;
   * does the function run for each sample, every time a control changes,
   * for each buffer, etc...
   */
  using control_policy = ossia::safe_nodes::last_tick;

  /**
   * The arguments here must match the specification given above,
   * else you'll get compile errors.
   */
  static void
  run(const ossia::audio_port& p1, // Input audio port
      float g,                     // Input gain control
      ossia::audio_port& p2,       // Output audio port
      ossia::token_request t,      // Timing information
      ossia::exec_state_facade st  // Global information (sample rate, etc.)
  )
  {
    // Allocate outputs
    const auto chans = p1.samples.size();
    p2.samples.resize(chans);

    // When do we start writing, and for how many samples
    const int64_t N = t.physical_write_duration(st.modelToSamples());
    const int64_t first_pos = t.physical_start(st.modelToSamples());

    // Process the input buffer
    const double gain = g;
    for (std::size_t i = 0; i < chans; i++)
    {
      auto& in = p1.samples[i];
      auto& out = p2.samples[i];

      const int64_t samples = in.size();
      int64_t max = std::min(N, samples);

      out.resize(samples);

      for (int64_t j = first_pos; j < max; j++)
      {
        // Very crude distortion
        using namespace std;
        out[j] = tanh(in[j] * gain);
      }
    }
  }
};
}

/**
 * score looks for a type named Node at top level, thus we
 * add a small typedef.
 */
using Node = MyAudioEffect::Distortion;
