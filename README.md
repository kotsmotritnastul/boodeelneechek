# будильничек
помните был такой плеер aimp3? самой удобной функцией там была даже не сам плеер а будильничек полноценный аналог которому я так и не смог найти - ни среди пк-программ ни даже среди андроид приложений, хотя андроид-приложения не подходят потому что мобильник неспособен выдать достаточно громкий звук чтобы меня разбудить.

за что я так любил будильничек из aimp3?
1. позволял установить в качестве звонка любой mp3-файл на компьютере.
1. позволял установить время не парясь о дате - если в сегодняшнем дне время будильничка уже прошло то он зазвонит в установленное время завтра.
1. позволял не оглушать сразу сильной громкостью, а плавно её наращивал - с удобным мне шагом.

в общем мне хочется отдельную программу которая бы делала то же самое в линуксе, а то из под вайна aimp3 всё хуже и хуже работает, да и не хочется целый плеер только ради маленького будильничка держать. ну и хотелось бы иметь нативное линукосовое приложение.

вот с этой целью я и сделал эту репу - тут будет лежать маленькая (около 20 килобайт) утилита которая повторяет функционал будильничка из aimp3 и только его, и ко всему прочему нативно запускается из под линукса.

## как пользоваться будильничком

1. копируете к себе репу через `git clone https://github.com/kotsmotritnastul/boodeelneechek.git`
1. создаёте в папке репы файл `where_is_music_file.txt`
1. кладёте туда путь к mp3-файлу который должен вас будить
1. запускаете скрипт `run.bash` в консоли который и скомпилирует и запустит будильничек
1. будильничек спросит когда ему вас будить
1. вводите время в формате `HH-MM` где `H` это часы а `M` это минуты
1. будильничек проверит правильно ли вы всё ввели, правильный ли путь к mp3-файлу, можно ли этот файл открыть вообще и сядет ждать когда подойдёт время вас будить
1. когда время придёт будильничек начнёт проигрывать mp3-файл постепенно повышая громкость до 100%
1. когда mp3-файл закончится будильничек закроется

## зависимости

скорее всего потребуется установить библиотеку `sdl2` для убунты и федоры вот тут https://github.com/libsdl-org/SDL/blob/main/docs/README-linux.md есть подробная иструкция, мне было достаточно просто скопировать команды оттуда.

если лень ходить по ссылке то вот команда для убунты

```
sudo apt-get install build-essential git make \
pkg-config cmake ninja-build gnome-desktop-testing libasound2-dev libpulse-dev \
libaudio-dev libfribidi-dev libjack-dev libsndio-dev libx11-dev libxext-dev \
libxrandr-dev libxcursor-dev libxfixes-dev libxi-dev libxss-dev libxtst-dev \
libxkbcommon-dev libdrm-dev libgbm-dev libgl1-mesa-dev libgles2-mesa-dev \
libegl1-mesa-dev libdbus-1-dev libibus-1.0-dev libudev-dev libthai-dev libusb-1.0-0-dev \
libpipewire-0.3-dev libwayland-dev libdecor-0-dev liburing-dev
```

а вот для федоры

```
sudo dnf install gcc git-core make cmake \
alsa-lib-devel fribidi-devel pulseaudio-libs-devel pipewire-devel \
libX11-devel libXext-devel libXrandr-devel libXcursor-devel libXfixes-devel \
libXi-devel libXScrnSaver-devel libXtst-devel dbus-devel ibus-devel \
systemd-devel mesa-libGL-devel libxkbcommon-devel mesa-libGLES-devel \
mesa-libEGL-devel vulkan-devel wayland-devel wayland-protocols-devel \
libdrm-devel mesa-libgbm-devel libusb1-devel libdecor-devel \
pipewire-jack-audio-connection-kit-devel libthai-devel \
liburing-devel zlib-ng-compat-static sdl2-compat-devel SDL2_mixer-devel
```

если чего-то не хватает - пишите в issues.
несмотря на то что я хотел сделать в первую очередь линуксовую утилиту - мне бы хотелось получить обратную связь про то как она запускается под виндой - если вообще запускается. у меня винды нет но если у вас есть - попробуйте, вдруг утилита выйдет действительно кроссплатформенной.
