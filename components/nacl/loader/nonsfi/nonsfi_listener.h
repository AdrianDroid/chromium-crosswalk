// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_NACL_LOADER_NONSFI_NONSFI_LISTENER_H_
#define COMPONENTS_NACL_LOADER_NONSFI_NONSFI_LISTENER_H_

#include <map>
#include <utility>

#include "base/macros.h"

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/waitable_event.h"
#include "base/threading/thread.h"
#include "components/nacl/common/nacl_types.h"
#include "ipc/ipc_listener.h"

namespace IPC {
class Message;
class SyncChannel;
}  // namespace IPC

class NaClTrustedListener;

namespace nacl {

struct NaClStartParams;

namespace nonsfi {

class NonSfiListener : public IPC::Listener {
 public:
  NonSfiListener();
  ~NonSfiListener() override;

  // Listen for a request to launch a non-SFI NaCl module.
  void Listen();
  bool Send(IPC::Message* msg);

 private:
  bool OnMessageReceived(const IPC::Message& msg) override;
  void OnAddPrefetchedResource(
      const nacl::NaClResourcePrefetchResult& prefetched_resource_file);
  void OnStart(const nacl::NaClStartParams& params);

  base::Thread io_thread_;
  base::WaitableEvent shutdown_event_;
  scoped_ptr<IPC::SyncChannel> channel_;
  scoped_refptr<NaClTrustedListener> trusted_listener_;

  scoped_ptr<std::map<std::string, int>> key_fd_map_;

  DISALLOW_COPY_AND_ASSIGN(NonSfiListener);
};

}  // namespace nonsfi
}  // namespace nacl

#endif  // COMPONENTS_NACL_LOADER_NONSFI_NONSFI_LISTENER_H_
