#!/bin/bash

#########################################
# usage:    hooeek username [-option]
# examples: hooeek L29Ah
#           hooeek L29Ah -m
#########################################

get_pages_number()
{
    local post_per_page=20
    local pages=$(($1 / $post_per_page))
    [[ "$(($1 % $post_per_page))" != "0" ]] && let pages++
    echo "$pages"
    return $pages
}

username=$1
main_page="`curl -s http://juick.com/${username}/`"
messages=`grep -P -o '(?<=(Messages:\ ))([0-9]+)' <<< $main_page`
pages=`get_pages_number $messages`
last_page=`curl -s "http://juick.com/${username}/?show=blog&page=$pages"`
first_post_number=`echo "$last_page" |\
                   grep -P "${username}/([0-9]{1,8})\"([^&])#\1" -o |\
                   sed -r -e 's/.*#(.*)/\1/' |\
                   tail -n 1`
first_post=`curl -s "http://juick.com/${username}/${first_post_number}"`
first_post=`echo $first_post | sed -r -e 's/All.*Messages.*//'`
first_post_date_all=`grep -P -o '\<span\ title(.*?)\<\/span' <<< $first_post`
first_post_date_nearly=`echo "$first_post_date_all" |\
                        sed -r -e 's/.*>(.*)<.*/\1/'`
first_post_date_exactly=`echo "$first_post_date_all" |\
                         sed -r -e 's/.*"(.*GMT).*/\1/'`

if [[ "$#" -eq 1 ]]; then
    echo "messages= $messages"
    echo "pages= $pages"
    echo "first_post_number= $first_post_number"
    echo "first_post_date_nearly= $first_post_date_nearly"
    echo "first_post_date_exactly= $first_post_date_exactly" 
    exit 
fi  

case "$2" in
    -m) #messages number
        echo "$messages"
    ;;
    -p) #pages number
        echo "$pages"
    ;;
    -n) #first post number
        echo "$first_post_number"
    ;;
    -d) #first post date
        echo "$first_post_date_exactly ($first_post_date_nearly)" 
    ;;
    *) 
        break
    ;;
esac

