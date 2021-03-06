// Copyright (c) 2020 The Orbit Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TIMER_TRACK_H_
#define TIMER_TRACK_H_

#include <map>
#include <memory>

#include "BlockChain.h"
#include "CallstackTypes.h"
#include "EventTrack.h"
#include "TextBox.h"
#include "Threading.h"
#include "TimerChain.h"
#include "Track.h"
#include "capture_data.pb.h"

class TextRenderer;

class TimerTrack : public Track {
 public:
  explicit TimerTrack(TimeGraph* time_graph);
  ~TimerTrack() override = default;

  // Pickable
  void Draw(GlCanvas* canvas, PickingMode picking_mode) override;
  void OnTimer(const orbit_client_protos::TimerInfo& timer_info);
  [[nodiscard]] std::string GetTooltip() const override;

  // Track
  void UpdatePrimitives(uint64_t min_tick, uint64_t max_tick,
                        PickingMode /*picking_mode*/) override;
  [[nodiscard]] Type GetType() const override { return kTimerTrack; }
  [[nodiscard]] float GetHeight() const override;

  [[nodiscard]] std::vector<std::shared_ptr<TimerChain>> GetTimers() override;
  [[nodiscard]] uint32_t GetDepth() const { return depth_; }
  [[nodiscard]] std::string GetExtraInfo(
      const orbit_client_protos::TimerInfo& timer);

  [[nodiscard]] Color GetColor() const;
  [[nodiscard]] static Color GetColor(ThreadID a_TID);
  [[nodiscard]] uint32_t GetNumTimers() const { return num_timers_; }
  [[nodiscard]] TickType GetMinTime() const { return min_time_; }
  [[nodiscard]] TickType GetMaxTime() const { return max_time_; }

  [[nodiscard]] const TextBox* GetFirstAfterTime(TickType time,
                                                 uint32_t depth) const;
  [[nodiscard]] const TextBox* GetFirstBeforeTime(TickType time,
                                                  uint32_t depth) const;

  // Must be overriden by child class for sensible behavior.
  [[nodiscard]] virtual const TextBox* GetLeft(TextBox* textbox) const {
    return textbox;
  };
  // Must be overriden by child class for sensible behavior.
  [[nodiscard]] virtual const TextBox* GetRight(TextBox* textbox) const {
    return textbox;
  };

  [[nodiscard]] virtual const TextBox* GetUp(TextBox* textbox) const;
  [[nodiscard]] virtual const TextBox* GetDown(TextBox* textbox) const;

  [[nodiscard]] std::vector<std::shared_ptr<TimerChain>> GetAllChains()
      override;
  [[nodiscard]] virtual bool IsEmpty() const;

  [[nodiscard]] bool IsCollapsable() const override { return depth_ > 1; }

  [[nodiscard]] float GetTextBoxHeight() const;

  virtual void UpdateBoxHeight();
  [[nodiscard]] virtual float GetYFromDepth(uint32_t depth) const;

 protected:
  [[nodiscard]] virtual bool IsTimerActive(
      const orbit_client_protos::TimerInfo& /*timer_info*/) const {
    return true;
  }
  [[nodiscard]] virtual Color GetTimerColor(
      const orbit_client_protos::TimerInfo& timer_info,
      bool is_selected) const = 0;
  [[nodiscard]] virtual bool TimerFilter(
      const orbit_client_protos::TimerInfo& /*timer_info*/) const {
    return true;
  }

  void UpdateDepth(uint32_t depth) {
    if (depth > depth_) depth_ = depth;
  }
  [[nodiscard]] std::shared_ptr<TimerChain> GetTimers(uint32_t depth) const;

  virtual void SetTimesliceText(const orbit_client_protos::TimerInfo& /*timer*/,
                                double /*elapsed_us*/, float /*min_x*/,
                                TextBox* /*text_box*/) {}
  TextRenderer* text_renderer_ = nullptr;
  uint32_t depth_ = 0;
  mutable Mutex mutex_;
  std::map<int, std::shared_ptr<TimerChain>> timers_;

  [[nodiscard]] virtual std::string GetBoxTooltip(PickingId id) const;
  float box_height_;
};

#endif  // ORBIT_GL_TIMER_TRACK_H_
