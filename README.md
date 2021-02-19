# [Sent Core][Sent-Core] – Official Messenger

This is the complete source code and the build instructions for the alpha version of the official server, based on the [Sent API][Sent-api].

[![Version](https://badge.fury.io/gh/hanriel%2Fsent-core.svg)](https://github.com/hanriel/sentcore/releases)
[![issuses](https://img.shields.io/github/issues/hanriel/Sent-Core)](https://github.com/hanriel/Sent-Core/issues)
[![issuses](https://img.shields.io/github/license/hanriel/Sent-Core)](https://github.com/hanriel/Sent-Core/license)

[![Preview of Telegram Desktop][preview_image]][preview_image_url]

The source code is published under GPLv3, the license is available [here][license].

## Supported systems

The latest version is available for

* [Windows 7 and above]()
* [macOS 10.12 and above]()
* [Linux static build for 64 bit]()

## Third-party

* OpenSSL 1.1.1 and 1.0.1 ([OpenSSL License](https://www.openssl.org/source/license.html))
* CMake ([New BSD License](https://github.com/Kitware/CMake/blob/master/Copyright.txt))

## Build instructions

* [Visual Studio 2019][msvc]
* [Xcode 12][xcode]
* [CMake on GNU/Linux][cmake]

[//]: # (LINKS)
[telegram]: https://telegram.org
[telegram_desktop]: https://desktop.telegram.org
[telegram_api]: https://core.telegram.org
[telegram_proto]: https://core.telegram.org/mtproto
[license]: LICENSE
[msvc]: docs/building-msvc.md
[xcode]: docs/building-xcode.md
[xcode_old]: docs/building-xcode-old.md
[cmake]: docs/building-cmake.md
[preview_image]: https://github.com/telegramdesktop/tdesktop/blob/dev/docs/assets/preview.png "Preview of Telegram Desktop"
[preview_image_url]: https://raw.githubusercontent.com/telegramdesktop/tdesktop/dev/docs/assets/preview.png
