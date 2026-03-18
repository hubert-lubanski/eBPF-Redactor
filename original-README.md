## [ZSO] Zadanie 2 - BPF REDACTOR
## Opis

Dodany został nowy typ programu **BPF_PROG_TYPE_REDACTOR** oraz nowy typ podpięcia **BPF_REDACTOR**.

#### procedury bpf_redactor_*
Procedury ```bpf_redactor_decide``` oraz ```bpf_redactor_redact``` są zadeklarowane w pliku ```inlclude/linux/redactor.h``` oraz zaimplementowane jako puste procedury.

```bpf_redactor_decide``` domyślnie przekazuje wartość **NO_REDACT**. Jeżeli program BPF zmieni przekazywaną wartość na dodatnią, otwarty plik zostanie oznaczony jako '_poddawany redakcji_'. Procedura ```bpf_redactor_redact``` jest wywolywana tylko dla plików aktualnie poddawanych redakcji.

Funkcja ```bpf_redactor_redact``` zapisuje log błędu, jeżeli zostanie wykonana bezpośrednio (powinna zostać podmieniona programem BPF).



#### procedury bpf_copy_*_buffer
Procedury ```bpf_copy_to_buffer``` oraz ```bpf_copy_from_buffer``` zaimplementowane są jako _bpf-helpers functions_ w pliku ```kernel/bpf/helpers.c``` i są dostępne tylko dla programów typu **BPF_PROG_TYPE_REDACTOR**.

W funkcjach ```bpf_copy_to/from_buffer``` wykorzystywany jest trick, gdzie wskaźnik na kontekst ```struct redactor_ctx```, jest wskaźnikiem do pola w większej strukturze ```struct packed_redactor_ctx``` dzięki czemu i zastosowaniu ```container_of``` możemy w prosty sposób przekazać przez program BPF wczytany bufor jedynie do naszych funkcji.

#### wywołania systemowe
Wywołania systemowe ```sys_count_redactions``` oraz ```sys_reset_redactions``` zaimplementowane są w pliku ```fs/file.c```. Korzystają z nowo dodanego pola ```atomic_long_t f_redacted_count``` w strukturze ```struct file```. Operacje dodawania nowych redakcji lub resetu są atomowe.

Zakładam, że redakcje przeprowadzone na pojedynczym pliku przez wiele procesów/wątków są kumulatywne, gdyż nie było to wyspecyfikowne w treści.


## Rozwiązanie
**Disclaimer:** <span style="font-size:0.9em;">_Nie używałem gita, więc moje rozwiązanie jest w formie zwykłego patacha._</span>

Rozwiązanie powinno się spokojnie nałożyć wywołując wewnątrz folderu z czystym jąderm polecenie
```
patch -d <kernel_directory> -p1 < solution.patch
```

Wszelkie zmiany są dodatkowo zaznaczone komentarzem
```
// CHANGED - ZSO 2024
```

W końcu pracowałem w całkiem ciekawej konfiguracji. Posiadałem cały katalog z plikami jądra w trybie *tylko do odczytu* oraz katalog roboczy zawierający dowiązania symboliczne do wszystkich katalogów i plików wewnątz katalogu jądra.

Chcąc edytować fragment, zastępowałem dowiązanie kopią. Jeżeli zastępowałem katalog, jego wnętrze nie było kopiowane, jedynie tworzone były dowiązania symboliczne do jego oryginalnej zawartości.

Dzięki temu ```diff``` działał ekstremalnie szybko oraz można było łatwo sprawdzić wszystkie _zmaterializowane_ pliki.

```cd <katalog kernela> && find . -mindepth 1 -type f ! -type l```
```
arch/x86/entry/syscalls/syscall_32.tbl
arch/x86/entry/syscalls/syscall_64.tbl
fs/file.c
fs/file_table.c
fs/open.c
fs/read_write.c
include/linux/bpf.h
include/linux/bpf_types.h
include/linux/fs.h
include/linux/redactor.h
include/linux/syscalls.h
include/uapi/asm-generic/unistd.h
include/uapi/linux/bpf.h
kernel/bpf/helpers.c
kernel/bpf/syscall.c
kernel/bpf/verifier.c
```



#### Statystyki
* Łącznie stworzyłem 77 patchy
* Maszyna nie zbootowała się tylko 2 razy
* Końcowa łatka to nieco ponad 450 linijek zmian




