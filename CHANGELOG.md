# Changelog

All notable changes to LotusVibe, the fork of
[fcitx5-lotus](https://github.com/LotusInputMethod/fcitx5-lotus), are recorded here. The format follows
[Keep a Changelog](https://keepachangelog.com/en/1.1.0/). Entries describe what changed for the user;
the reasoning and measurements behind each patch live in
[KHAC-GI-SO-VOI-BAN-GOC.md](KHAC-GI-SO-VOI-BAN-GOC.md).

The fork has no release numbers of its own yet. `CMakeLists.txt` still carries the upstream version it
was based on.

## [Unreleased]

## Fork baseline — 2026-09-26

`ban-dung` at `b121f3c`, based on upstream `dev` at `79d5706` (2026-09-15) plus the upstream commits
picked on 2026-09-24. Everything below is what the fork does differently from that upstream base.

### Changed

- The three uinput modes (Slow, Smooth, Super Smooth) are merged into a single **Uinput** mode.
  Saved configs, per-app rules (modes 1–3) and the mode order are migrated to it, and the mode menu
  lists it once (#6).
- After sending backspaces in Uinput mode, the addon waits for the app's surrounding-text update,
  with a timer, instead of sleeping a fixed time (`WaitSurroundingEvent`, on by default).
- Apps that declare no surrounding text (terminals, Chromium) skip the retry wait.
- The server sends backspaces back to back by default; `LOTUS_BACKSPACE_GAP_MS` (0–50) adds a gap.
- The Messenger and Facebook composer fixes are on by default (#5).
- Identifiers, comments and log strings in fork-only code are English and follow
  [AGENTS.md](AGENTS.md) (#4).

### Added

- Messenger and Facebook composers: the selected text is typed over with right Shift+Left instead of
  being deleted first, so the composer never empties (`MessengerSelectOvertype`). This applies to the
  post composer too. Keys typed during a timed-out selection are typed back.
- Messenger composer: no commit on a half-updated surrounding-text snapshot, and a short settle wait
  after the deletion shows done (`WaitSurroundingSettleMs` 40 ms, 60 ms for the first word).
- LibreOffice: Uinput mode deletes through surrounding text instead of backspace keys.
- Tray icon follows the GNOME Shell top bar colour. The icon path resolver, which upstream removed,
  is restored for the KDE Plasma panel.
- Hardened systemd unit for the server.
- The server honours `LOTUS_SOCKET_NAMESPACE` like the addon, and `LOTUS_SERVER_PATH` names the server
  the monitor expects (taken from CMake instead of a hardcoded `/usr/bin`).
- Tests: property tests over random key sequences, held-key repeat in Smooth
  (LotusInputMethod/fcitx5-lotus#472), the KDE panel icon (LotusInputMethod/fcitx5-lotus#374), and a
  spell-check test that uses the dictionary in the source tree.

### Removed

- `FixUinputWithAck` and the Chromium suggestion workaround.

### Fixed

- The server drains the libinput queue on every loop iteration, so pending events are not left
  waiting (LotusInputMethod/fcitx5-lotus#507).
- The anti-duplication guard for autofill suggestions applies only in browser address bars.

### Documentation

- [KHAC-GI-SO-VOI-BAN-GOC.md](KHAC-GI-SO-VOI-BAN-GOC.md): every patch, why it exists, and its
  upstream status.
- [AGENTS.md](AGENTS.md): rules for changing this fork and sending patches upstream (#3).
- README: install steps for another machine, the single Uinput mode, restarting the server after an
  update.
