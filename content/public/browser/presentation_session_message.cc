// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/public/browser/presentation_session_message.h"

namespace content {

PresentationSessionMessage::PresentationSessionMessage(
    const std::string& presentation_url,
    const std::string& presentation_id,
    scoped_ptr<std::string> message)
    : presentation_url(presentation_url),
      presentation_id(presentation_id),
      message(message.Pass()),
      data(nullptr) {
}

PresentationSessionMessage::PresentationSessionMessage(
    const std::string& presentation_url,
    const std::string& presentation_id,
    scoped_ptr<std::vector<uint8_t>> data)
    : presentation_url(presentation_url),
      presentation_id(presentation_id),
      message(nullptr),
      data(data.Pass()) {
}

// static
scoped_ptr<PresentationSessionMessage>
PresentationSessionMessage::CreateStringMessage(
    const std::string& presentation_url,
    const std::string& presentation_id,
    scoped_ptr<std::string> message) {
  return scoped_ptr<PresentationSessionMessage>(new PresentationSessionMessage(
      presentation_url, presentation_id, message.Pass()));
}

// static
scoped_ptr<PresentationSessionMessage>
PresentationSessionMessage::CreateBinaryMessage(
    const std::string& presentation_url,
    const std::string& presentation_id,
    scoped_ptr<std::vector<uint8_t>> data) {
  return scoped_ptr<PresentationSessionMessage>(new PresentationSessionMessage(
      presentation_url, presentation_id, data.Pass()));
}

bool PresentationSessionMessage::is_binary() const {
  return data != nullptr;
}

PresentationSessionMessage::~PresentationSessionMessage() {
}

}  // namespace content
