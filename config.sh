#!/bin/bash

# Yeah I wrote a config script so what
# Run it with ./config.sh

MODULES="C_LEXER CLI_OPTS ARRAYLIST"
MODULE_NAMES="C Lexer|CLI option handler|ArrayLists"
ENABLED="n y y"

set -e

IDX=0

CLEANED_UP=0
cleanup() {
    if [ $CLEANED_UP -eq 0 ]; then
        tput cnorm
        stty echo
        tput rmcup
        CLEANED_UP=1
    fi
}

trap cleanup EXIT INT TERM
tput civis
tput smcup

load_from_file() {
    ENABLED=""
    for module in $MODULES; do
        value="n"

        if grep -q "^CONFIG_$module=" config.mk; then
            value=$(grep "^CONFIG_$module=" config.mk | cut -d'=' -f2)
        fi

        ENABLED="$ENABLED $value"
    done
    ENABLED=$(echo "$ENABLED" | sed 's/^ //')
}

if [ -f "config.mk" ]; then
    load_from_file
fi

# draw() {
#     clear
#     IFS='|' read -r -a modules <<< "$MODULE_NAMES"
#     for i in ${!modules[@]}; do
#         state=$(echo "$ENABLED" | cut -d' ' -f$((i+1)))
#         prefix="  "
#         [ $i -eq $IDX ] && prefix="> "
# 
#         [ "$state" = "y" ] && state="X" || state=" "
# 
#         name="${modules[$i]}"
#         printf "%s[%s] %s\n" "$prefix" "$state" "$name"
#     done
# 
#     printf "\nArrow keys up/down to move | Enter to toggle option | Press 'C' to save and exit\n"
# }

draw_line() {
    local i=$1
    local modules
    IFS='|' read -r -a modules <<< "$MODULE_NAMES"

    state=$(echo "$ENABLED" | cut -d' ' -f$((i+1)))
    [ "$state" = "y" ] && state="X" || state=" "
    
    prefix="  "
    [ "$i" -eq "$IDX" ] && prefix="> "

    tput cup "$(($i+2))" 0
    printf "    %s[%s] %s" "$prefix" "$state" "${modules[$i]}"
    tput el

    tput cup "$((${#modules[@]}+2))" 0
}

draw_all() {
    clear
    printf "  StarlitNova's C Library build configuration"
    IFS='|' read -r -a modules <<< "$MODULE_NAMES"
    for i in "${!modules[@]}"; do
        draw_line "$i"
    done

    tput cup "$((${#modules[@]}+1))" 0
    printf "\n\n  Arrow keys up/down to move | Enter to toggle option | Press 'C' to save and exit | Press 'Q' to cancel\n"
}

toggle() {
    i=0
    new=""
    for s in $ENABLED; do
        if [ $i -eq $IDX ]; then
            [ "$s" = "y" ] && s="n" || s="y"
        fi
        new="$new $s"
        i=$((i+1))
    done
    ENABLED=$(echo "$new" | sed 's/^ //')
}

draw_all

while true; do
    old_idx=$IDX
    
    read -rsn1 key
    case "$key" in
        $'\x1b') # escape
            read -rsn2 key
            [ "$key" = "[A" ] && IDX=$((IDX-1)) # up arrow
            [ "$key" = "[B" ] && IDX=$((IDX+1)) # down arrow
            ;;
        "") # enter
            toggle
            ;;
        [Cc])
            break
            ;;
        [Qq])
            exit
            ;;
    esac

    [ $IDX -lt 0 ] && IDX=0
    max=$(($(echo "$MODULES" | wc -w)-1))
    [ $IDX -gt $max ] && IDX=$max

    draw_line $old_idx
    draw_line $IDX
done

save() {
    i=0
    rm -f config.mk
    for m in $MODULES; do
        s=$(echo "$ENABLED" | cut -d' ' -f$((i+1)))
        echo "CONFIG_$m=$s" >> config.mk
        i=$((i+1))
    done

    cleanup

    echo "Saved to config.mk"
}

save
