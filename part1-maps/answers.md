# Частина 1. Відповіді

Середовище: Ubuntu 26.04 LTS, `uname -r`: 7.0.0-22-generic, архітектура процесору: x86_64.

## Адреси та області

| Що                      | Адреса           | Рядок maps (діапазон, права, назва)                                          | Область        |
| ------------------------| -----------------|----------------------------------------------------------------------------- | -------------- |
| main                    | `0x5ccf7bd16000` | `5ccf7bd16000-5ccf7bd17000 r-xp 00001000 00:35 141 /vagrant/part1-maps/maps` | Code (text)    |
| global_initialized      | `0x5ccf7bd19000` | `5ccf7bd19000-5ccf7bd1a000 rw-p 00003000 00:35 141 /vagrant/part1-maps/maps` | Data           |
| global_uninitialized    | `0x5ccf7bd19000` | `5ccf7bd19000-5ccf7bd1a000 rw-p 00003000 00:35 141 /vagrant/part1-maps/maps` | BSS            |
| рядковий літерал        | `0x5ccf7bd15000` | `5ccf7bd15000-5ccf7bd16000 r--p 00000000 00:35 141 /vagrant/part1-maps/maps` | Read-only Data |
| local (main)            | `0x7ffc3c47b3e4` | `7ffc3c45d000-7ffc3c47e000 rw-p 00000000 00:00 0 [stack]`                    | Stack          |
| local_in_nested         | `0x7ffc3c47b3e4` | `7ffc3c45d000-7ffc3c47e000 rw-p 00000000 00:00 0 [stack]`                    | Stack          |
| small (100 байт)        | `0x5ccf947c7000` | `5ccf947c7000-5ccf947e8000 rw-p 00000000 00:00 0 [heap]`                     | Heap           |
| big (4 МіБ)             | `0x5ccf947c7000` | `5ccf947c7000-5ccf947e8000 rw-p 00000000 00:00 0 [heap]`                     | Heap           |
