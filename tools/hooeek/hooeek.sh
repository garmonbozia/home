#!/bin/bash

if [ "$1" == "--help" ]; then
    echo -e "\n\033[1mUsage:\033[0m\n"
    echo -e "\thooeek [username|post_number] [-option]\n\n"
    echo -e "\n\033[1mExamples:\033[0m\n"
    echo -e "\thooeek ugnich -m       get messages number"
    echo -e "\thooeek ugnich -r       get user's readers" 
    echo -e "\thooeek ugnich -s       get who's user subscribed to"
    echo -e "\thooeek ugnich -p       get pages number"
    echo -e "\thooeek ugnich -n       get first post #number"
    echo -e "\thooeek ugnich -d       get first post date"
    echo -e "\thooeek ugnich -t       top user's posts"
    echo -e "\thooeek ugnich -c       count comments in all"
    echo -e "\t                           user's posts"
    echo -e "\thooeek \"#1356453\" -!"    
    echo -e "\thooeek 1356453 -!      get post recommenders\n\n"
    exit 
fi  

username=$1

get_messages_number()
{
    echo "`curl -s http://juick.com/${username}/ | grep -P -o '(?<=(Messages:\ ))([0-9]+)'`"
}
 
get_pages_number()
{
    local post_per_page=20
    local pages=$(($1 / $post_per_page))
    [[ "$(($1 % $post_per_page))" != "0" ]] && let pages++
    echo "$pages"
}

get_first_post_number()
{
    echo "${last_page}" | grep -P "${username}/([0-9]{1,8})\"([^&])#\1" -o | sed -r -e 's/.*#(.*)/\1/' | tail -n 1
}

get_recommenders()
{
    post_number=`echo "$1" | sed -e 's/#//'`
    echo `wget http://juick.com/${post_number} -q -O- | grep -E "Recommended by.*(\@.*)<\/a>" -o | sed -r -e 's/Recommended by //' -e 's/<\/a>//g' -e 's/<a href([^<]*)\">//g'`
}

get_readers()
{
    curl -s http://juick.com/${username}/readers | grep -E '<h2>My readers.*(.*)<\/a>' -o | sed -r -e 's/<\/?h2>//g' -e 's/<p>/: /' -e 's/<\/a>//g' -e 's/<a href([^<]*)\">/@/g' -e "s/My/@${username}/"
}

get_subscribe()
{
    curl -s http://juick.com/${username}/friends | grep -E '<h2>I read.*(.*)<\/a>' -o | sed -r -e 's/<\/?h2>//g' -e 's/<p>/: /' -e 's/<\/a>//g' -e 's/<a href([^<]*)\">/@/g' -e "s/I/@${username}/"
    
}

if [[ "$#" -eq 1 ]]; then
    echo -e "ERROR: too few arguments\nTry hooeek --help"
    exit 
fi  

if [ "$(curl -s http://juick.com/${username} | grep -E "<h1>Page Not Found</h1>" -o)" != "" ] ; then
    echo -e "ERROR: No such user\nTry hooeek --help"
    exit
fi

case "$2" in
    -m) #messages number
        get_messages_number
        exit
    ;;
    -p) #pages number
        get_pages_number $(get_messages_number)
        exit
    ;;
    -n) #first post number
        last_page=`curl -s "http://juick.com/${username}/?show=blog&page=$(get_pages_number $(get_messages_number))"`
        get_first_post_number
        exit
    ;;
    -d) #first post date
        last_page=`curl -s "http://juick.com/${username}/?show=blog&page=$(get_pages_number $(get_messages_number))"`
        echo `curl -s "http://juick.com/${username}/$(get_first_post_number)" | sed -r -e 's/All.*Messages.*//'` | grep -P -o '\<span\ title(.*?)\<\/span' | sed -r -e 's/.*"(.*GMT).*/\1/' | head -n 1
        exit
    ;;
    -!)
        get_recommenders $1
        exit
    ;;
    -r) 
        get_readers
        exit
    ;;
    -s) 
        get_subscribe
        exit
    ;;
esac

messages=`get_messages_number`
pages=`get_pages_number ${messages}`
 
# (search top posts)
if [[ "$2" == "-t" ]]; then
    echo "* start searching top @${username} posts"
    [[ "$3" == "" ]] && pages_first=1     || pages_first=$3
    [[ "$4" == "" ]] && pages_last=$pages || pages_last=$4
    for page in `seq ${pages_first} ${pages_last}`; do
        echo "* page ${page}/${pages_last}" 
        nth_page=`curl -s "http://juick.com/${username}/?show=blog&page=${page}"`
        posts=`echo "${nth_page}" |\
               grep -P "${username}/([0-9]{1,8})\"([^&])#\1" -o |\
               sed -r -e 's/.*#(.*)/\1/'`
        for post in $posts; do
            recomm=`get_recommenders ${post}`
            top_recomm="`echo ${recomm} | grep -w '@top'`"
            [[ "${top_recomm}" != "" ]] && echo "${post}"
        done
    done
    echo "done"
    exit
fi
 
# if -c (count totally replies number)
if [[ "$2" == "-c" ]]; then
    page=$pages
    while [[ $page -gt "0" ]]; do
        wget -q -O- "http://juick.com/${username}/?show=blog&page=${page}"
        let page--
    done | grep -o "[0-9]* replies" | awk '{i+=$1}END{print i}'
            # ^ weird ^ wooga ^ shit ^
    exit
fi

