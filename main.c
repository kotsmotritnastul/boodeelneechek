#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <time.h>
#include <stdio.h>

#ifdef _DEBUG
#define DEBUG_DIVIDER 10
#else
#define DEBUG_DIVIDER 1
#endif

#define MILLISECONDS_IN_SECOND 1000
#define ON_THIS_PERCENT_OF_DURATION_MUSIC_SHOULD_HAVE_MAXIMUM_VOLUME 33
#define USER_TYPED_IN_WRONG_FORMAT 0
#define USER_INPUT_CANNOT_BE_READ_AT_ALL -1
#define GET_CURRENT_MUSIC_VOLUME -1
#define MAXIMUM_MUSIC_VOLUME 128
#define HOW_MUCH_TIMES_TO_PLAY_MP3_FILE 1

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    #define sleep_seconds(x) Sleep((x) * MILLISECONDS_IN_SECOND)
#else
    #include <unistd.h>
    #define sleep_seconds(x) sleep(x)
#endif

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    #define milliseconds_sleep(ms) Sleep(ms)
#else
    #include <time.h>
    #include <unistd.h>

    void milliseconds_sleep(long milliseconds) {
        struct timespec ts;
        ts.tv_sec = milliseconds / MILLISECONDS_IN_SECOND;
        ts.tv_nsec = (milliseconds % MILLISECONDS_IN_SECOND) * 1000000;
        nanosleep(&ts, NULL);
    }
#endif

int main() {
#ifdef _DEBUG
    printf("DEBUG MODE\n");
#endif
    char wakeup_time_string[5];
    int wakeup_hour, wakeup_minute;
    printf("Введите время когда будильничку звонить в формате HH-MM\nгде H это часы а M это минуты\n");
    scanf("%5s", wakeup_time_string);

#ifdef _DEBUG
    printf("Функция scanf записала в переменную %s\n", wakeup_time_string);
#endif
    int sscanf_result = sscanf(wakeup_time_string, "%2d-%2d", &wakeup_hour, &wakeup_minute);

    if  (
        sscanf_result == USER_TYPED_IN_WRONG_FORMAT ||
        sscanf_result == USER_INPUT_CANNOT_BE_READ_AT_ALL ||
        wakeup_hour > 23 ||
        wakeup_minute > 59
        )
    {
        printf("Неправильный формат. Проверьте что ввели время когда будильник должен звонить в формате HH-MM где H это часы а M это минуты, что количество часов от 00 до 23 а количество минут от 00 до 59.\n");
        exit(1);
    }

    FILE *path_to_music_file_as_file = fopen("where_is_music_file.txt", "r");
    if (path_to_music_file_as_file == NULL) {
        printf("Не удалось открыть файл \"where_is_music_file.txt\". Он должен лежать в одной папке с программой, первой строчкой в нём должен быть путь к mp3-файлу который будильничек должен проигрывать когда придёт указанное вами время.\n");
        exit(1);
    }
    char path_to_music_file_as_string[255];
    if (fscanf(path_to_music_file_as_file, "%254[^\n]", path_to_music_file_as_string) == 1) {
        printf("Путь к mp3-файлу который будильничек должен проигрывать когда придёт указанное вами время успешно прочитан и это \"%s\"\n", path_to_music_file_as_string);
        FILE *pointer_to_mp3_file = fopen(path_to_music_file_as_string, "r");
        if ( pointer_to_mp3_file != NULL )
        {
            printf("Mp3-файл \"%s\" действительно существует и его можно открыть.\n", path_to_music_file_as_string);
            fclose(pointer_to_mp3_file);
        }
        else
        {
            printf("Mp3-файл \"%s\" невозможно открыть.\nПуть точно указан правильно?\nПрава доступа к файлу точно не сбились?\n", path_to_music_file_as_string);
            exit(1);
        }
    }
    else
    {
        printf("То ли файл где должен лежать путь к mp3-файлу пустой, то ли первую строчку из него не удалось прочитать.\n");
        exit(1);
    }
    fclose(path_to_music_file_as_file);








    time_t how_long_wait_before_wake_up_in_seconds;
    time_t raw_time = time(NULL);

    struct tm *current_time_tm = localtime(&raw_time);
    struct tm current_time = *current_time_tm;
    struct tm wakeup_time = *current_time_tm;

#ifdef _DEBUG
    printf("Время сейчас: %02d:%02d:%02d\n",
            current_time.tm_hour,
            current_time.tm_min,
            current_time.tm_sec);

    printf("Дата сейчас: %04d-%02d-%02d\n",
            current_time.tm_year + 1900,
            current_time.tm_mon + 1,
            current_time.tm_mday);
#endif
    // если час в который надо проснуться меньше чем текущий или и час и минута пробуждения меньше текущих - значит будильничек должен звонить завтра
    if  (
        wakeup_time.tm_hour > wakeup_hour ||
        ( wakeup_time.tm_hour >= wakeup_hour && wakeup_time.tm_min > wakeup_minute )
        )
    {
        wakeup_time.tm_mday = wakeup_time.tm_mday + 1;
        wakeup_time.tm_hour = wakeup_hour;
    }
    else
    {
        wakeup_time.tm_hour = wakeup_hour;
    }

    wakeup_time.tm_min = wakeup_minute;

#ifdef _DEBUG
    printf("Время когда будильничек зазвонит: %02d:%02d:%02d\n",
            wakeup_time.tm_hour,
            wakeup_time.tm_min,
            wakeup_time.tm_sec);
    printf("Дата когда будильничек зазвонит: %04d-%02d-%02d\n",
            wakeup_time.tm_year + 1900,
            wakeup_time.tm_mon + 1,
            wakeup_time.tm_mday);
#endif

    how_long_wait_before_wake_up_in_seconds = difftime(mktime(&wakeup_time), mktime(&current_time));
    printf("Через сколько секунд будильничек зазвонит: %ld\n", how_long_wait_before_wake_up_in_seconds / DEBUG_DIVIDER);
    sleep_seconds(how_long_wait_before_wake_up_in_seconds / DEBUG_DIVIDER);
    printf("Будильничек начинает звонить.\n");







    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    Mix_Music *music = Mix_LoadMUS(path_to_music_file_as_string);
    int mp3_file_duration = (int)Mix_MusicDuration(music) * MILLISECONDS_IN_SECOND;
    int how_much_duration_already_passed = 0;

#ifdef _DEBUG
    printf("Полная длительность mp3-файла %d секунд.\n", mp3_file_duration / MILLISECONDS_IN_SECOND);
    printf("Полная громкость будет через %.0f секунд.\n", (double)mp3_file_duration / MILLISECONDS_IN_SECOND * ON_THIS_PERCENT_OF_DURATION_MUSIC_SHOULD_HAVE_MAXIMUM_VOLUME / 100);
#endif

    Mix_VolumeMusic(0);

    Mix_PlayMusic(music, HOW_MUCH_TIMES_TO_PLAY_MP3_FILE);

    for (int mp3_file_volume = 0; mp3_file_volume <= MAXIMUM_MUSIC_VOLUME; mp3_file_volume++)
        {
        int how_long_sleep = MILLISECONDS_IN_SECOND * (int)Mix_MusicDuration(music) * ON_THIS_PERCENT_OF_DURATION_MUSIC_SHOULD_HAVE_MAXIMUM_VOLUME / 100 / MAXIMUM_MUSIC_VOLUME;
#ifdef _DEBUG
        printf("Шаг через который будет повышаться громкость %d миллисекунд.\n", how_long_sleep);
#endif
        milliseconds_sleep(how_long_sleep);
        Mix_VolumeMusic(mp3_file_volume);
        how_much_duration_already_passed = how_much_duration_already_passed + how_long_sleep;

#ifdef _DEBUG
        printf("Музыка играет уже %d секунд.\n", how_much_duration_already_passed / MILLISECONDS_IN_SECOND);
        printf("Громкость сейчас %d%%\n", 100 * Mix_VolumeMusic(GET_CURRENT_MUSIC_VOLUME) / MAXIMUM_MUSIC_VOLUME);
#endif
#ifndef _DEBUG
        printf("\rГромкость сейчас %d%%", 100 * Mix_VolumeMusic(GET_CURRENT_MUSIC_VOLUME) / MAXIMUM_MUSIC_VOLUME);
        fflush(stdout);
#endif

        if ( how_much_duration_already_passed > mp3_file_duration )
            {
            break;
            }

        }
        printf("\n");

    int how_long_wait_before_stop_playing_mp3 = mp3_file_duration - how_much_duration_already_passed;
    if ( how_long_wait_before_stop_playing_mp3 <= 0 )
        {
            how_long_wait_before_stop_playing_mp3 = 0;
        }
    SDL_Delay(how_long_wait_before_stop_playing_mp3);

    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();




    return 0;
}
