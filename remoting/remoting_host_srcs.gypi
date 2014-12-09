# Copyright 2014 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'remoting_host_sources': [
      'host/audio_capturer.cc',
      'host/audio_capturer.h',
      'host/audio_capturer_linux.cc',
      'host/audio_capturer_linux.h',
      'host/audio_capturer_mac.cc',
      'host/audio_capturer_win.cc',
      'host/audio_capturer_win.h',
      'host/audio_scheduler.cc',
      'host/audio_scheduler.h',
      'host/audio_silence_detector.cc',
      'host/audio_silence_detector.h',
      'host/basic_desktop_environment.cc',
      'host/basic_desktop_environment.h',
      'host/branding.cc',
      'host/branding.h',
      'host/capture_scheduler.cc',
      'host/capture_scheduler.h',
      'host/chromeos/aura_desktop_capturer.cc',
      'host/chromeos/aura_desktop_capturer.h',
      'host/chromeos/clipboard_aura.cc',
      'host/chromeos/clipboard_aura.h',
      'host/chromeos/message_box.cc',
      'host/chromeos/message_box.h',
      'host/chromeos/mouse_cursor_monitor_aura.cc',
      'host/chromeos/mouse_cursor_monitor_aura.h',
      'host/chromeos/point_transformer.cc',
      'host/chromeos/point_transformer.h',
      'host/chromeos/skia_bitmap_desktop_frame.cc',
      'host/chromeos/skia_bitmap_desktop_frame.h',
      'host/chromium_port_allocator_factory.cc',
      'host/chromium_port_allocator_factory.h',
      'host/chromoting_host.cc',
      'host/chromoting_host.h',
      'host/chromoting_host_context.cc',
      'host/chromoting_host_context.h',
      'host/chromoting_messages.cc',
      'host/chromoting_messages.h',
      'host/chromoting_param_traits.cc',
      'host/chromoting_param_traits.h',
      'host/client_session.cc',
      'host/client_session.h',
      'host/client_session_control.h',
      'host/clipboard.h',
      'host/clipboard_mac.mm',
      'host/clipboard_win.cc',
      'host/clipboard_x11.cc',
      'host/config_file_watcher.cc',
      'host/config_file_watcher.h',
      'host/config_watcher.h',
      'host/constants_mac.cc',
      'host/constants_mac.h',
      'host/continue_window.cc',
      'host/continue_window.h',
      'host/continue_window_chromeos.cc',
      'host/continue_window_linux.cc',
      'host/continue_window_mac.mm',
      'host/continue_window_win.cc',
      'host/daemon_process.cc',
      'host/daemon_process.h',
      'host/daemon_process_win.cc',
      'host/desktop_environment.h',
      'host/desktop_process.cc',
      'host/desktop_process.h',
      'host/desktop_resizer.h',
      'host/desktop_resizer_linux.cc',
      'host/desktop_session.cc',
      'host/desktop_session.h',
      'host/desktop_session_agent.cc',
      'host/desktop_session_agent.h',
      'host/desktop_session_win.cc',
      'host/desktop_session_win.h',
      'host/desktop_resizer_mac.cc',
      'host/desktop_resizer_win.cc',
      'host/desktop_session_connector.h',
      'host/desktop_session_proxy.cc',
      'host/desktop_session_proxy.h',
      'host/desktop_shape_tracker.h',
      'host/desktop_shape_tracker_mac.cc',
      'host/desktop_shape_tracker_win.cc',
      'host/desktop_shape_tracker_linux.cc',
      'host/disconnect_window_chromeos.cc',
      'host/disconnect_window_linux.cc',
      'host/disconnect_window_mac.h',
      'host/disconnect_window_mac.mm',
      'host/disconnect_window_win.cc',
      'host/dns_blackhole_checker.cc',
      'host/dns_blackhole_checker.h',
      'host/gnubby_auth_handler.h',
      'host/gnubby_auth_handler_posix.cc',
      'host/gnubby_auth_handler_posix.h',
      'host/gnubby_auth_handler_win.cc',
      'host/gnubby_socket.cc',
      'host/gnubby_socket.h',
      'host/heartbeat_sender.cc',
      'host/heartbeat_sender.h',
      'host/host_change_notification_listener.cc',
      'host/host_change_notification_listener.h',
      'host/host_config.cc',
      'host/host_config_constants.cc',
      'host/host_config.h',
      'host/host_event_logger.h',
      'host/host_event_logger_posix.cc',
      'host/host_event_logger_win.cc',
      'host/host_exit_codes.cc',
      'host/host_exit_codes.h',
      'host/host_export.h',
      'host/host_extension.h',
      'host/host_extension_session.cc',
      'host/host_extension_session.h',
      'host/host_extension_session_manager.cc',
      'host/host_extension_session_manager.h',
      'host/host_secret.cc',
      'host/host_secret.h',
      'host/host_status_logger.cc',
      'host/host_status_logger.h',
      'host/host_status_monitor.h',
      'host/host_status_observer.h',
      'host/host_window.h',
      'host/host_window_proxy.cc',
      'host/host_window_proxy.h',
      'host/input_injector.h',
      'host/input_injector_chromeos.cc',
      'host/input_injector_chromeos.h',
      'host/input_injector_mac.cc',
      'host/input_injector_win.cc',
      'host/input_injector_x11.cc',
      'host/ipc_audio_capturer.cc',
      'host/ipc_audio_capturer.h',
      'host/ipc_constants.cc',
      'host/ipc_constants.h',
      'host/ipc_desktop_environment.cc',
      'host/ipc_desktop_environment.h',
      'host/ipc_host_event_logger.cc',
      'host/ipc_host_event_logger.h',
      'host/ipc_input_injector.cc',
      'host/ipc_input_injector.h',
      'host/ipc_mouse_cursor_monitor.cc',
      'host/ipc_mouse_cursor_monitor.h',
      'host/ipc_screen_controls.cc',
      'host/ipc_screen_controls.h',
      'host/ipc_util.h',
      'host/ipc_util_posix.cc',
      'host/ipc_util_win.cc',
      'host/ipc_video_frame_capturer.cc',
      'host/ipc_video_frame_capturer.h',
      'host/it2me_desktop_environment.cc',
      'host/it2me_desktop_environment.h',
      'host/linux/audio_pipe_reader.cc',
      'host/linux/audio_pipe_reader.h',
      'host/linux/unicode_to_keysym.cc',
      'host/linux/unicode_to_keysym.h',
      'host/linux/x11_util.cc',
      'host/linux/x11_util.h',
      'host/linux/x_server_clipboard.cc',
      'host/linux/x_server_clipboard.h',
      'host/local_input_monitor.h',
      'host/local_input_monitor_chromeos.cc',
      'host/local_input_monitor_mac.mm',
      'host/local_input_monitor_win.cc',
      'host/local_input_monitor_x11.cc',
      'host/logging.h',
      'host/logging_posix.cc',
      'host/logging_win.cc',
      'host/me2me_desktop_environment.cc',
      'host/me2me_desktop_environment.h',
      'host/mouse_clamping_filter.cc',
      'host/mouse_clamping_filter.h',
      'host/oauth_token_getter.cc',
      'host/oauth_token_getter.h',
      'host/pairing_registry_delegate.cc',
      'host/pairing_registry_delegate.h',
      'host/pairing_registry_delegate_linux.cc',
      'host/pairing_registry_delegate_linux.h',
      'host/pairing_registry_delegate_mac.cc',
      'host/pairing_registry_delegate_win.cc',
      'host/pairing_registry_delegate_win.h',
      'host/pam_authorization_factory_posix.cc',
      'host/pam_authorization_factory_posix.h',
      'host/pin_hash.cc',
      'host/pin_hash.h',
      'host/policy_hack/policy_watcher.cc',
      'host/policy_hack/policy_watcher.h',
      'host/policy_hack/policy_watcher_chromeos.cc',
      'host/policy_hack/policy_watcher_linux.cc',
      'host/policy_hack/policy_watcher_mac.mm',
      'host/policy_hack/policy_watcher_win.cc',
      'host/register_support_host_request.cc',
      'host/register_support_host_request.h',
      'host/remote_input_filter.cc',
      'host/remote_input_filter.h',
      'host/remoting_me2me_host.cc',
      'host/resizing_host_observer.cc',
      'host/resizing_host_observer.h',
      'host/sas_injector.h',
      'host/sas_injector_win.cc',
      'host/screen_controls.h',
      'host/screen_resolution.cc',
      'host/screen_resolution.h',
      'host/server_log_entry_host.cc',
      'host/server_log_entry_host.h',
      'host/session_manager_factory.cc',
      'host/session_manager_factory.h',
      'host/shaped_desktop_capturer.cc',
      'host/shaped_desktop_capturer.h',
      'host/signaling_connector.cc',
      'host/signaling_connector.h',
      'host/single_window_desktop_environment.cc',
      'host/single_window_desktop_environment.h',
      'host/single_window_input_injector.h',
      'host/single_window_input_injector_linux.cc',
      'host/single_window_input_injector_mac.cc',
      'host/single_window_input_injector_win.cc',
      'host/token_validator_base.cc',
      'host/token_validator_base.h',
      'host/token_validator_factory_impl.cc',
      'host/token_validator_factory_impl.h',
      'host/usage_stats_consent.h',
      'host/usage_stats_consent_mac.cc',
      'host/usage_stats_consent_win.cc',
      'host/username.cc',
      'host/username.h',
      'host/video_frame_recorder.cc',
      'host/video_frame_recorder.h',
      'host/video_frame_recorder_host_extension.cc',
      'host/video_frame_recorder_host_extension.h',
      'host/video_scheduler.cc',
      'host/video_scheduler.h',
      'host/win/com_imported_mstscax.tlh',
      'host/win/com_security.cc',
      'host/win/com_security.h',
      'host/win/launch_process_with_token.cc',
      'host/win/launch_process_with_token.h',
      'host/win/omaha.cc',
      'host/win/omaha.h',
      'host/win/rdp_client.cc',
      'host/win/rdp_client.h',
      'host/win/rdp_client_window.cc',
      'host/win/rdp_client_window.h',
      'host/win/security_descriptor.cc',
      'host/win/security_descriptor.h',
      'host/win/session_desktop_environment.cc',
      'host/win/session_desktop_environment.h',
      'host/win/session_input_injector.cc',
      'host/win/session_input_injector.h',
      'host/win/window_station_and_desktop.cc',
      'host/win/window_station_and_desktop.h',
      'host/win/wts_terminal_monitor.cc',
      'host/win/wts_terminal_monitor.h',
      'host/win/wts_terminal_observer.h',
    ],
    'remoting_cast_sources' : [
      'host/cast_extension.cc',
      'host/cast_extension.h',
      'host/cast_extension_session.cc',
      'host/cast_extension_session.h',
      'host/cast_video_capturer_adapter.cc',
      'host/cast_video_capturer_adapter.h',
    ],
    'remoting_it2me_host_static_sources' : [
      'host/it2me/it2me_host.cc',
      'host/it2me/it2me_host.h',
      'host/it2me/it2me_native_messaging_host.cc',
      'host/it2me/it2me_native_messaging_host.h',
    ],
  }
}
