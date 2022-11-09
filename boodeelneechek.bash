#!/bin/bash
gromkost=$(awk -F"[][]" '/Left:/ { print $2 }' <(amixer sget Master)) #тут мы получаем текущую громкость в системе и присваиваем её значение переменной gromkost
maxgromkost=99% #тут мы устанавливаем громкость выше которой задирать громкость уже не надо
pactl set-sink-volume @DEFAULT_SINK@ 1% #тут мы устанавливаем громкость на 1% чтобы будильничек не глушил меня сразу как рыбу динамитом

echo -n "Введите время пробуждения в формате чч:мм:сс "
read wakeuptime

#следующие три строчки нужны для того чтобы скрипт понял когда ему меня будить
current_epoch=$(date +%s) #тут скрипт узнаёт текущее время
target_epoch=$(date -d "$wakeuptime" +%s) #тут скрипт узнаёт в какое время меня надо будить
sleep_seconds=$(( $target_epoch - $current_epoch )) #тут скрипт узнаёт сколько ему времени ждать с момента запуска скрипта до момента когда меня надо будить

sleep $sleep_seconds #тут скрипт ждёт обозначенное выше время (в секундах)

play "/ПУТЬ/ДО/ФАЙЛА.mp3" & #тут начинает играть музыка, амперсанд очень важен чтобы одновременно с музыкой начиналась повышаться громкость

#этот цикл повышает громкость до тех пор пока она не станет равна максимальной, которая указана выше
while [[ $gromkost != $maxgromkost ]]
do
    sleep 0.6 #тут выставлено такое время чтобы громкость повышалась в течении 0,6*100%=60 секунд
    gromkost=$(awk -F"[][]" '/Left:/ { print $2 }' <(amixer sget Master)) #каждый раз надо переприсваивать переменной громкости значение текущей громкости, чтобы при каждой итерации цикл понимал надо ему ещё её повышать или хватит уже
    pactl set-sink-volume @DEFAULT_SINK@ +1% #эта команда повышает громкость на 1%
done
