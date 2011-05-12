#!/bin/bash
 
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
    echo "${last_page}" | grep -P "${username}/([0-9]{1,8})\"([^&])#\1" -o |\
sed -r -e 's/.*#(.*)/\1/' |\
tail -n 1
}

get_recommenders()
{
    post_number=`echo "$1" | sed -e 's/#//'`
    echo `wget http://juick.com/${post_number} -q -O- | grep -E "Recommended by.*(\@.*)<\/a>" -o | sed -r -e 's/Recommended by //' -e 's/<\/a>//g' -e 's/<a href([^<]*)\">//g'`
}

case "$2" in
    -m) #messages number
        echo `get_messages_number`
        exit
    ;;
    -p) #pages number
        get_pages_number `get_messages_number`
        exit
    ;;
    -n) #first post number
        messages=`get_messages_number`
        pages=`get_pages_number ${messages}`
        last_page=`curl -s "http://juick.com/${username}/?show=blog&page=${pages}"`
        echo `get_first_post_number`
        exit
    ;;
    -d) #first post date
        messages=`get_messages_number`
        pages=`get_pages_number ${messages}`
        last_page=`curl -s "http://juick.com/${username}/?show=blog&page=${pages}"`
        first_post_number=`get_first_post_number`
        first_post=`curl -s "http://juick.com/${username}/${first_post_number}"`
        first_post=`echo ${first_post} | sed -r -e 's/All.*Messages.*//'`
        echo ${first_post} | grep -P -o '\<span\ title(.*?)\<\/span' | sed -r -e 's/.*"(.*GMT).*/\1/'
        exit
    ;;
    -r)
        get_recommenders $1
        exit
    ;;
esac


messages=`get_messages_number`
pages=`get_pages_number ${messages}`
last_page=`curl -s "http://juick.com/${username}/?show=blog&page=${pages}"`
first_post_number=`get_first_post_number`
first_post=`curl -s "http://juick.com/${username}/${first_post_number}"`
first_post=`echo ${first_post} | sed -r -e 's/All.*Messages.*//'`
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
    [[ "$3" == "" ]] && pages_first=1     || pages_first=$3
    [[ "$4" == "" ]] && pages_last=$pages || pages_last=$4

    page=$pages_last
    while [[ $page -gt $(($pages_first-1)) ]]; do
        wget -q -O- "http://juick.com/${username}/?show=blog&page=${page}"
        let page--
    done | grep -o "[0-9]* replies" | awk '{i+=$1}END{print i}'
# ^ weird ^ wooga ^ shit ^
    exit
fi
