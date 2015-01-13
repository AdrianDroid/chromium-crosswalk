// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// MSVC++ requires this to be set before any other includes to get M_PI.
#define _USE_MATH_DEFINES

#include "content/browser/renderer_host/input/web_input_event_util.h"

#include <cmath>

#include "base/strings/string_util.h"
#include "content/common/input/web_touch_event_traits.h"
#include "ui/events/event_constants.h"
#include "ui/events/gesture_detection/gesture_event_data.h"
#include "ui/events/gesture_detection/motion_event.h"
#include "ui/gfx/geometry/safe_integer_conversions.h"

using blink::WebGestureEvent;
using blink::WebInputEvent;
using blink::WebTouchEvent;
using blink::WebTouchPoint;
using ui::MotionEvent;

namespace {

const char* GetKeyIdentifier(ui::KeyboardCode key_code) {
  switch (key_code) {
    case ui::VKEY_MENU:
      return "Alt";
    case ui::VKEY_CONTROL:
      return "Control";
    case ui::VKEY_SHIFT:
      return "Shift";
    case ui::VKEY_CAPITAL:
      return "CapsLock";
    case ui::VKEY_LWIN:
    case ui::VKEY_RWIN:
      return "Win";
    case ui::VKEY_CLEAR:
      return "Clear";
    case ui::VKEY_DOWN:
      return "Down";
    case ui::VKEY_END:
      return "End";
    case ui::VKEY_RETURN:
      return "Enter";
    case ui::VKEY_EXECUTE:
      return "Execute";
    case ui::VKEY_F1:
      return "F1";
    case ui::VKEY_F2:
      return "F2";
    case ui::VKEY_F3:
      return "F3";
    case ui::VKEY_F4:
      return "F4";
    case ui::VKEY_F5:
      return "F5";
    case ui::VKEY_F6:
      return "F6";
    case ui::VKEY_F7:
      return "F7";
    case ui::VKEY_F8:
      return "F8";
    case ui::VKEY_F9:
      return "F9";
    case ui::VKEY_F10:
      return "F10";
    case ui::VKEY_F11:
      return "F11";
    case ui::VKEY_F12:
      return "F12";
    case ui::VKEY_F13:
      return "F13";
    case ui::VKEY_F14:
      return "F14";
    case ui::VKEY_F15:
      return "F15";
    case ui::VKEY_F16:
      return "F16";
    case ui::VKEY_F17:
      return "F17";
    case ui::VKEY_F18:
      return "F18";
    case ui::VKEY_F19:
      return "F19";
    case ui::VKEY_F20:
      return "F20";
    case ui::VKEY_F21:
      return "F21";
    case ui::VKEY_F22:
      return "F22";
    case ui::VKEY_F23:
      return "F23";
    case ui::VKEY_F24:
      return "F24";
    case ui::VKEY_HELP:
      return "Help";
    case ui::VKEY_HOME:
      return "Home";
    case ui::VKEY_INSERT:
      return "Insert";
    case ui::VKEY_LEFT:
      return "Left";
    case ui::VKEY_NEXT:
      return "PageDown";
    case ui::VKEY_PRIOR:
      return "PageUp";
    case ui::VKEY_PAUSE:
      return "Pause";
    case ui::VKEY_SNAPSHOT:
      return "PrintScreen";
    case ui::VKEY_RIGHT:
      return "Right";
    case ui::VKEY_SCROLL:
      return "Scroll";
    case ui::VKEY_SELECT:
      return "Select";
    case ui::VKEY_UP:
      return "Up";
    case ui::VKEY_DELETE:
      return "U+007F";  // Standard says that DEL becomes U+007F.
    case ui::VKEY_MEDIA_NEXT_TRACK:
      return "MediaNextTrack";
    case ui::VKEY_MEDIA_PREV_TRACK:
      return "MediaPreviousTrack";
    case ui::VKEY_MEDIA_STOP:
      return "MediaStop";
    case ui::VKEY_MEDIA_PLAY_PAUSE:
      return "MediaPlayPause";
    case ui::VKEY_VOLUME_MUTE:
      return "VolumeMute";
    case ui::VKEY_VOLUME_DOWN:
      return "VolumeDown";
    case ui::VKEY_VOLUME_UP:
      return "VolumeUp";
    default:
      return NULL;
  };
}

WebInputEvent::Type ToWebInputEventType(MotionEvent::Action action) {
  switch (action) {
    case MotionEvent::ACTION_DOWN:
      return WebInputEvent::TouchStart;
    case MotionEvent::ACTION_MOVE:
      return WebInputEvent::TouchMove;
    case MotionEvent::ACTION_UP:
      return WebInputEvent::TouchEnd;
    case MotionEvent::ACTION_CANCEL:
      return WebInputEvent::TouchCancel;
    case MotionEvent::ACTION_POINTER_DOWN:
      return WebInputEvent::TouchStart;
    case MotionEvent::ACTION_POINTER_UP:
      return WebInputEvent::TouchEnd;
  }
  NOTREACHED() << "Invalid MotionEvent::Action.";
  return WebInputEvent::Undefined;
}

// Note that |is_action_pointer| is meaningful only in the context of
// |ACTION_POINTER_UP| and |ACTION_POINTER_DOWN|; other actions map directly to
// WebTouchPoint::State.
WebTouchPoint::State ToWebTouchPointState(const MotionEvent& event,
                                          size_t pointer_index) {
  switch (event.GetAction()) {
    case MotionEvent::ACTION_DOWN:
      return WebTouchPoint::StatePressed;
    case MotionEvent::ACTION_MOVE:
      return WebTouchPoint::StateMoved;
    case MotionEvent::ACTION_UP:
      return WebTouchPoint::StateReleased;
    case MotionEvent::ACTION_CANCEL:
      return WebTouchPoint::StateCancelled;
    case MotionEvent::ACTION_POINTER_DOWN:
      return static_cast<int>(pointer_index) == event.GetActionIndex()
                 ? WebTouchPoint::StatePressed
                 : WebTouchPoint::StateStationary;
    case MotionEvent::ACTION_POINTER_UP:
      return static_cast<int>(pointer_index) == event.GetActionIndex()
                 ? WebTouchPoint::StateReleased
                 : WebTouchPoint::StateStationary;
  }
  NOTREACHED() << "Invalid MotionEvent::Action.";
  return WebTouchPoint::StateUndefined;
}

WebTouchPoint CreateWebTouchPoint(const MotionEvent& event,
                                  size_t pointer_index) {
  WebTouchPoint touch;
  touch.id = event.GetPointerId(pointer_index);
  touch.state = ToWebTouchPointState(event, pointer_index);
  touch.position.x = event.GetX(pointer_index);
  touch.position.y = event.GetY(pointer_index);
  touch.screenPosition.x = event.GetRawX(pointer_index);
  touch.screenPosition.y = event.GetRawY(pointer_index);

  // A note on touch ellipse specifications:
  //
  // Android MotionEvent provides the major and minor axes of the touch ellipse,
  // as well as the orientation of the major axis clockwise from vertical, in
  // radians. See:
  // http://developer.android.com/reference/android/view/MotionEvent.html
  //
  // The proposed extension to W3C Touch Events specifies the touch ellipse
  // using two radii along x- & y-axes and a positive acute rotation angle in
  // degrees. See:
  // http://dvcs.w3.org/hg/webevents/raw-file/default/touchevents.html

  float major_radius = event.GetTouchMajor(pointer_index) / 2.f;
  float minor_radius = event.GetTouchMinor(pointer_index) / 2.f;
  float orientation_deg = event.GetOrientation(pointer_index) * 180.f / M_PI;
  DCHECK_GE(major_radius, 0);
  DCHECK_GE(minor_radius, 0);
  DCHECK_GE(major_radius, minor_radius);
  // Allow a small bound tolerance to account for floating point conversion.
  DCHECK_GT(orientation_deg, -90.01f);
  DCHECK_LT(orientation_deg, 90.01f);
  if (orientation_deg >= 0) {
    // The case orientation_deg == 0 is handled here on purpose: although the
    // 'else' block is equivalent in this case, we want to pass the 0 value
    // unchanged (and 0 is the default value for many devices that don't
    // report elliptical touches).
    touch.radiusX = minor_radius;
    touch.radiusY = major_radius;
    touch.rotationAngle = orientation_deg;
  } else {
    touch.radiusX = major_radius;
    touch.radiusY = minor_radius;
    touch.rotationAngle = orientation_deg + 90;
  }

  touch.force = event.GetPressure(pointer_index);

  return touch;
}

}  // namespace

namespace content {

void UpdateWindowsKeyCodeAndKeyIdentifier(blink::WebKeyboardEvent* event,
                                          ui::KeyboardCode windows_key_code) {
  event->windowsKeyCode = windows_key_code;

  const char* id = GetKeyIdentifier(windows_key_code);
  if (id) {
    base::strlcpy(event->keyIdentifier, id, sizeof(event->keyIdentifier) - 1);
  } else {
    base::snprintf(event->keyIdentifier,
                   sizeof(event->keyIdentifier),
                   "U+%04X",
                   base::ToUpperASCII(static_cast<int>(windows_key_code)));
  }
}

blink::WebTouchEvent CreateWebTouchEventFromMotionEvent(
    const ui::MotionEvent& event,
    bool may_cause_scrolling) {
  static_assert(static_cast<int>(MotionEvent::MAX_TOUCH_POINT_COUNT) ==
                    static_cast<int>(blink::WebTouchEvent::touchesLengthCap),
                "inconsistent maximum number of active touch points");

  blink::WebTouchEvent result;

  WebTouchEventTraits::ResetType(
      ToWebInputEventType(event.GetAction()),
      (event.GetEventTime() - base::TimeTicks()).InSecondsF(),
      &result);
  result.causesScrollingIfUncanceled = may_cause_scrolling;

  result.modifiers = EventFlagsToWebEventModifiers(event.GetFlags());
  result.touchesLength =
      std::min(event.GetPointerCount(),
               static_cast<size_t>(WebTouchEvent::touchesLengthCap));
  DCHECK_GT(result.touchesLength, 0U);

  for (size_t i = 0; i < result.touchesLength; ++i)
    result.touches[i] = CreateWebTouchPoint(event, i);

  return result;
}

WebGestureEvent CreateWebGestureEvent(const ui::GestureEventDetails& details,
                                      base::TimeDelta timestamp,
                                      const gfx::PointF& location,
                                      const gfx::PointF& raw_location,
                                      int flags) {
  WebGestureEvent gesture;
  gesture.timeStampSeconds = timestamp.InSecondsF();
  gesture.x = gfx::ToFlooredInt(location.x());
  gesture.y = gfx::ToFlooredInt(location.y());
  gesture.globalX = gfx::ToFlooredInt(raw_location.x());
  gesture.globalY = gfx::ToFlooredInt(raw_location.y());
  gesture.modifiers = EventFlagsToWebEventModifiers(flags);
  gesture.sourceDevice = blink::WebGestureDeviceTouchscreen;

  switch (details.type()) {
    case ui::ET_GESTURE_SHOW_PRESS:
      gesture.type = WebInputEvent::GestureShowPress;
      gesture.data.showPress.width = details.bounding_box_f().width();
      gesture.data.showPress.height = details.bounding_box_f().height();
      break;
    case ui::ET_GESTURE_DOUBLE_TAP:
      gesture.type = WebInputEvent::GestureDoubleTap;
      DCHECK_EQ(1, details.tap_count());
      gesture.data.tap.tapCount = details.tap_count();
      gesture.data.tap.width = details.bounding_box_f().width();
      gesture.data.tap.height = details.bounding_box_f().height();
      break;
    case ui::ET_GESTURE_TAP:
      gesture.type = WebInputEvent::GestureTap;
      DCHECK_GE(details.tap_count(), 1);
      gesture.data.tap.tapCount = details.tap_count();
      gesture.data.tap.width = details.bounding_box_f().width();
      gesture.data.tap.height = details.bounding_box_f().height();
      break;
    case ui::ET_GESTURE_TAP_UNCONFIRMED:
      gesture.type = WebInputEvent::GestureTapUnconfirmed;
      DCHECK_EQ(1, details.tap_count());
      gesture.data.tap.tapCount = details.tap_count();
      gesture.data.tap.width = details.bounding_box_f().width();
      gesture.data.tap.height = details.bounding_box_f().height();
      break;
    case ui::ET_GESTURE_LONG_PRESS:
      gesture.type = WebInputEvent::GestureLongPress;
      gesture.data.longPress.width = details.bounding_box_f().width();
      gesture.data.longPress.height = details.bounding_box_f().height();
      break;
    case ui::ET_GESTURE_LONG_TAP:
      gesture.type = WebInputEvent::GestureLongTap;
      gesture.data.longPress.width = details.bounding_box_f().width();
      gesture.data.longPress.height = details.bounding_box_f().height();
      break;
    case ui::ET_GESTURE_TWO_FINGER_TAP:
      gesture.type = blink::WebInputEvent::GestureTwoFingerTap;
      gesture.data.twoFingerTap.firstFingerWidth = details.first_finger_width();
      gesture.data.twoFingerTap.firstFingerHeight =
          details.first_finger_height();
      break;
    case ui::ET_GESTURE_SCROLL_BEGIN:
      gesture.type = WebInputEvent::GestureScrollBegin;
      gesture.data.scrollBegin.deltaXHint = details.scroll_x_hint();
      gesture.data.scrollBegin.deltaYHint = details.scroll_y_hint();
      break;
    case ui::ET_GESTURE_SCROLL_UPDATE:
      gesture.type = WebInputEvent::GestureScrollUpdate;
      gesture.data.scrollUpdate.deltaX = details.scroll_x();
      gesture.data.scrollUpdate.deltaY = details.scroll_y();
      gesture.data.scrollUpdate.previousUpdateInSequencePrevented =
          details.previous_scroll_update_in_sequence_prevented();
      break;
    case ui::ET_GESTURE_SCROLL_END:
      gesture.type = WebInputEvent::GestureScrollEnd;
      break;
    case ui::ET_SCROLL_FLING_START:
      gesture.type = WebInputEvent::GestureFlingStart;
      gesture.data.flingStart.velocityX = details.velocity_x();
      gesture.data.flingStart.velocityY = details.velocity_y();
      break;
    case ui::ET_SCROLL_FLING_CANCEL:
      gesture.type = WebInputEvent::GestureFlingCancel;
      break;
    case ui::ET_GESTURE_PINCH_BEGIN:
      gesture.type = WebInputEvent::GesturePinchBegin;
      break;
    case ui::ET_GESTURE_PINCH_UPDATE:
      gesture.type = WebInputEvent::GesturePinchUpdate;
      gesture.data.pinchUpdate.scale = details.scale();
      break;
    case ui::ET_GESTURE_PINCH_END:
      gesture.type = WebInputEvent::GesturePinchEnd;
      break;
    case ui::ET_GESTURE_TAP_CANCEL:
      gesture.type = WebInputEvent::GestureTapCancel;
      break;
    case ui::ET_GESTURE_TAP_DOWN:
      gesture.type = WebInputEvent::GestureTapDown;
      gesture.data.tapDown.width = details.bounding_box_f().width();
      gesture.data.tapDown.height = details.bounding_box_f().height();
      break;
    case ui::ET_GESTURE_BEGIN:
    case ui::ET_GESTURE_END:
    case ui::ET_GESTURE_SWIPE:
      // The caller is responsible for discarding these gestures appropriately.
      gesture.type = WebInputEvent::Undefined;
      break;
    default:
      NOTREACHED() << "ui::EventType provided wasn't a valid gesture event: "
                   << details.type();
  }

  return gesture;
}

WebGestureEvent CreateWebGestureEventFromGestureEventData(
    const ui::GestureEventData& data) {
  return CreateWebGestureEvent(data.details,
                               data.time - base::TimeTicks(),
                               gfx::PointF(data.x, data.y),
                               gfx::PointF(data.raw_x, data.raw_y),
                               data.flags);
}

int EventFlagsToWebEventModifiers(int flags) {
  int modifiers = 0;

  if (flags & ui::EF_SHIFT_DOWN)
    modifiers |= blink::WebInputEvent::ShiftKey;
  if (flags & ui::EF_CONTROL_DOWN)
    modifiers |= blink::WebInputEvent::ControlKey;
  if (flags & ui::EF_ALT_DOWN)
    modifiers |= blink::WebInputEvent::AltKey;
  if (flags & ui::EF_COMMAND_DOWN)
    modifiers |= blink::WebInputEvent::MetaKey;

  if (flags & ui::EF_LEFT_MOUSE_BUTTON)
    modifiers |= blink::WebInputEvent::LeftButtonDown;
  if (flags & ui::EF_MIDDLE_MOUSE_BUTTON)
    modifiers |= blink::WebInputEvent::MiddleButtonDown;
  if (flags & ui::EF_RIGHT_MOUSE_BUTTON)
    modifiers |= blink::WebInputEvent::RightButtonDown;
  if (flags & ui::EF_CAPS_LOCK_DOWN)
    modifiers |= blink::WebInputEvent::CapsLockOn;
  if (flags & ui::EF_IS_REPEAT)
    modifiers |= blink::WebInputEvent::IsAutoRepeat;
  if (flags & ui::EF_NUMPAD_KEY)
    modifiers |= blink::WebInputEvent::IsKeyPad;

  return modifiers;
}

int WebEventModifiersToEventFlags(int modifiers) {
  int flags = 0;

  if (modifiers & blink::WebInputEvent::ShiftKey)
    flags |= ui::EF_SHIFT_DOWN;
  if (modifiers & blink::WebInputEvent::ControlKey)
    flags |= ui::EF_CONTROL_DOWN;
  if (modifiers & blink::WebInputEvent::AltKey)
    flags |= ui::EF_ALT_DOWN;
  if (modifiers & blink::WebInputEvent::MetaKey)
    flags |= ui::EF_COMMAND_DOWN;

  if (modifiers & blink::WebInputEvent::LeftButtonDown)
    flags |= ui::EF_LEFT_MOUSE_BUTTON;
  if (modifiers & blink::WebInputEvent::MiddleButtonDown)
    flags |= ui::EF_MIDDLE_MOUSE_BUTTON;
  if (modifiers & blink::WebInputEvent::RightButtonDown)
    flags |= ui::EF_RIGHT_MOUSE_BUTTON;
  if (modifiers & blink::WebInputEvent::CapsLockOn)
    flags |= ui::EF_CAPS_LOCK_DOWN;
  if (modifiers & blink::WebInputEvent::IsAutoRepeat)
    flags |= ui::EF_IS_REPEAT;
  if (modifiers & blink::WebInputEvent::IsKeyPad)
    flags |= ui::EF_NUMPAD_KEY;

  return flags;
}

}  // namespace content
