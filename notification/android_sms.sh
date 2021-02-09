#/bin/bash

phonenum=$1
smsbody=$2

# use adb to say "start the app we use for SMS"
# with the following information
adb shell am start -a android.intent.action.SENDTO -d sms:$phonenum  --es sms_body $smsbody --ez exit_on_sent true
#adb shell am start -a android.intent.action.SENDTO -d sms:3128880068 --es sms_body testtest  --ez exit_on_sent true

# wait for the GUI to do stuff
sleep 1

# press key 22 (D-pad Right)
adb shell input keyevent 22

# wait for the GUI to do stuff
sleep 1

# press key 66 (Enter)
adb shell input keyevent 66

# wait for the GUI to do stuff
sleep 1

# press home key
adb shell input keyevent 3

#for key maping: https://stackoverflow.com/questions/7789826/adb-shell-input-events


# Note: This solution kind of sucks ass.
'
Better would be to actually build an Android app
that does something like this:
SmsManager sms = SmsManager.getDefault();
sms.sendTextMessage(NUMBER, null, MESSAGE, null, null);
'
