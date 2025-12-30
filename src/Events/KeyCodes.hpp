#pragma once

// clang-format off
enum class KeyCode : int {
  Unknown = 0,
  // Digits
  D0 = 48, D1, D2, D3, D4, D5, D6, D7, D8, D9,
  // Alpha
  A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
  // Controls
  Space = 32, Escape = 256, Enter = 257, Tab = 258, Backspace = 259,
  // Arrows
  Right = 262, Left = 263, Down = 264, Up = 265,
  // Function keys
  F1 = 290, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
};

enum KeyMod : int {
  None     = 0,
  Shift    = 1 << 0,
  Control  = 1 << 1,
  Alt      = 1 << 2,
  Super    = 1 << 3,
  CapsLock = 1 << 4,
  NumLock  = 1 << 5
};
// clang-format on
