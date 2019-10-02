#!/bin/bash
EXPECTED_ARGS=4

if [ $EXPECTED_ARGS -ne $# ]
then
    echo wrong number of arguments - 4 needed
fi

if [ $4 -gt $3 ]
then
    echo we cant have more levels than directories
fi

mkdir $1

a=$(echo $(( $3/$4 )))
b=$(echo $(( $3%$4 ))) #an exw upoloipo 8a dwsw kai sto telos kapoia dirs mexri level=upoloipo
files=1 #metrhths gia ta files pou exw ftiaksei wste otan exw auta pou 8elw na mh ftiaksw alla

for (( c=1; c<=$a; c++ ))
do
    if [ $files -le $2 ] #if files<=maxfiles ftiakse to file
    then
        r=$(echo $((1 + RANDOM % 131072))) #tuxaios ari8mos 1-128
        randstr=$(tr -cd '[:alnum:]'< /dev/urandom | head -c $r) #kanw random grammh mexri 128b
        echo $randstr >> $1/file$files #dhmiourgw kai grafw sto file
        files=$(echo $(( $files+1 ))) #enhmerwnw ton counter
    fi

    r=$(echo $((1 + RANDOM % 8))) #tuxaios ari8mos 1-8
    name=$(tr -cd '[:alnum:]' < /dev/urandom | head -c $r) #tuxaio onoma gia to directory me grammata kai ari8mous mege8ous 1-8
    mydir=$1/$c$name
    mkdir $mydir

    if [ $files -le $2 ] #if files<=maxfiles ftiakse to file
    then
        r=$(echo $((1 + RANDOM % 131072))) #tuxaios ari8mos 1-128
        randstr=$(tr -cd '[:alnum:]'< /dev/urandom | head -c $r) #kanw random grammh mexri 128b
        echo $randstr >> $mydir/file$files #dhmiourgw kai grafw sto file
        files=$(echo $(( $files+1 ))) #enhmerwnw ton counter
    fi

    for(( i=1; i<$4; i++ )) #ftiaxnw ena directory se ka8e level
    do
        r=$(echo $((1 + RANDOM % 8))) #tuxaios ari8mos 1-8
        name=$(tr -cd '[:alnum:]' < /dev/urandom | head -c $r) #tuxaio onoma gia to directory me grammata kai ari8mous mege8ous 1-8
        mydir=$mydir/$name
        mkdir $mydir

        if [ $files -le $2 ]
        then
            r=$(echo $((1 + RANDOM % 131072))) #kanw random grammh mexri 128b
            randstr=$(tr -cd '[:alnum:]'< /dev/urandom | head -c $r)
            echo $randstr >> $mydir/file$files
            files=$(echo $(( $files+1 )))
        fi

    done
done

#h periptwsh pou exw upoloipo
if [ $b -ne 0 ]
then
    r=$(echo $((1 + RANDOM % 8))) #tuxaios ari8mos 1-8
    name=$(tr -cd '[:alnum:]' < /dev/urandom | head -c $r) #tuxaio onoma gia to directory me grammata kai ari8mous mege8ous 1-8
    mydir=$1/$c$name
    mkdir $mydir

    if [ $files -le $2 ] #if files<=maxfiles ftiakse to file
    then
        r=$(echo $((1 + RANDOM % 131072))) #kanw random grammh mexri 128b
        randstr=$(tr -cd '[:alnum:]'< /dev/urandom | head -c $r)
        echo $randstr >> $mydir/file$files
        files=$(echo $(( $files+1 )))
    fi

    for (( j=1; j<$b; j++ ))
    do
        r=$(echo $((1 + RANDOM % 8))) #tuxaios ari8mos 1-8
        name=$(tr -cd '[:alnum:]' < /dev/urandom | head -c $r) #tuxaio onoma gia to directory me grammata kai ari8mous mege8ous 1-8
        mydir=$mydir/$name
        mkdir $mydir

        if [ $files -le $2 ] #if files<=maxfiles ftiakse to file
        then
            r=$(echo $((1 + RANDOM % 131072))) #kanw random grammh mexri 128b
            randstr=$(tr -cd '[:alnum:]'< /dev/urandom | head -c $r)
            echo $randstr >> $mydir/file$files
            files=$(echo $(( $files+1 )))
        fi
    done
fi

#an exw upoloipo arxeiwn ta vazw ola to arxiko dir
for(( i=$files; i<=$2; i++ ))
do
    r=$(echo $((1 + RANDOM % 131072))) #kanw random grammh mexri 128b
    randstr=$(tr -cd '[:alnum:]'< /dev/urandom | head -c $r)
    echo $randstr >> $1/file$i
done
