#!/bin/sh
# Перевіряє, що середовище придатне для ЛР2. Запуск: sh check_env.sh
status=0
need() {
    if command -v "$1" >/dev/null 2>&1; then
        echo "ok      $1"
    else
        echo "MISSING $1  ->  sudo apt install $2"
        status=1
    fi
}
need gcc build-essential
need make build-essential
need strace strace
need setarch util-linux
need /usr/bin/time time

[ -r /proc/self/maps ] && echo "ok      /proc/self/maps" || { echo "MISSING /proc/self/maps (це не Linux?)"; status=1; }

tmp=$(mktemp -d) || exit 1
printf '#include <stdlib.h>\nint main(void){char *p = malloc(4); p[4] = 1; return 0;}\n' > "$tmp/t.c"
if gcc -fsanitize=address -g "$tmp/t.c" -o "$tmp/t" 2>"$tmp/build.log" &&
    ! "$tmp/t" >"$tmp/asan.log" 2>&1 &&
    grep -q 'ERROR: AddressSanitizer: heap-buffer-overflow' "$tmp/asan.log"; then
    echo "ok      AddressSanitizer"
else
    echo "FAIL    AddressSanitizer: очікуваний heap-buffer-overflow не виявлено; див. діагностику нижче."
    cat "$tmp/build.log" "$tmp/asan.log" 2>/dev/null
    status=1
fi
rm -rf "$tmp"

echo "page size: $(getconf PAGESIZE), kernel: $(uname -r), arch: $(uname -m)"
[ $status -eq 0 ] && echo "Усе готово." || echo "Є проблеми, див. вище."
exit $status
