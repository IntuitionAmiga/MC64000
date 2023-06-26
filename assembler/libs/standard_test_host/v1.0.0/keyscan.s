
;  888b     d888  .d8888b.   .d8888b.      d8888  888    d8P
;  8888b   d8888 d88P  Y88b d88P  Y88b    d8P888  888   d8P
;  88888b.d88888 888    888 888          d8P 888  888  d8P
;  888Y88888P888 888        888d888b.   d8P  888  888d88K
;  888 Y888P 888 888        888P "Y88b d88   888  8888888b
;  888  Y8P  888 888    888 888    888 8888888888 888  Y88b
;  888   "   888 Y88b  d88P Y88b  d88P       888  888   Y88b
;  888       888  "Y8888P"   "Y8888P"        888  888    Y88b
;
;   - 64-bit 680x0-inspired Virtual Machine and assembler -
;
; Definitions for keyboard scan codes

    @equ KSC_RESERVED     0
    @equ KSC_ESC          1
    @equ KSC_1            2
    @equ KSC_2            3
    @equ KSC_3            4
    @equ KSC_4            5
    @equ KSC_5            6
    @equ KSC_6            7
    @equ KSC_7            8
    @equ KSC_8            9
    @equ KSC_9            10
    @equ KSC_0            11
    @equ KSC_MINUS        12
    @equ KSC_EQUAL        13
    @equ KSC_BACKSPACE    14
    @equ KSC_TAB          15
    @equ KSC_Q            16
    @equ KSC_W            17
    @equ KSC_E            18
    @equ KSC_R            19
    @equ KSC_T            20
    @equ KSC_Y            21
    @equ KSC_U            22
    @equ KSC_I            23
    @equ KSC_O            24
    @equ KSC_P            25
    @equ KSC_LEFTBRACE    26
    @equ KSC_RIGHTBRACE   27
    @equ KSC_ENTER        28
    @equ KSC_LEFTCTRL     29
    @equ KSC_A            30
    @equ KSC_S            31
    @equ KSC_D            32
    @equ KSC_F            33
    @equ KSC_G            34
    @equ KSC_H            35
    @equ KSC_J            36
    @equ KSC_K            37
    @equ KSC_L            38
    @equ KSC_SEMICOLON    39
    @equ KSC_APOSTROPHE   40
    @equ KSC_GRAVE        41
    @equ KSC_LEFTSHIFT    42
    @equ KSC_BACKSLASH    43
    @equ KSC_Z            44
    @equ KSC_X            45
    @equ KSC_C            46
    @equ KSC_V            47
    @equ KSC_B            48
    @equ KSC_N            49
    @equ KSC_M            50
    @equ KSC_COMMA        51
    @equ KSC_DOT          52
    @equ KSC_SLASH        53
    @equ KSC_RIGHTSHIFT   54
    @equ KSC_KPASTERISK   55
    @equ KSC_LEFTALT      56
    @equ KSC_SPACE        57
    @equ KSC_CAPSLOCK     58
    @equ KSC_F1           59
    @equ KSC_F2           60
    @equ KSC_F3           61
    @equ KSC_F4           62
    @equ KSC_F5           63
    @equ KSC_F6           64
    @equ KSC_F7           65
    @equ KSC_F8           66
    @equ KSC_F9           67
    @equ KSC_F10          68
    @equ KSC_NUMLOCK      69
    @equ KSC_SCROLLLOCK   70
    @equ KSC_KP7          71
    @equ KSC_KP8          72
    @equ KSC_KP9          73
    @equ KSC_KPMINUS      74
    @equ KSC_KP4          75
    @equ KSC_KP5          76
    @equ KSC_KP6          77
    @equ KSC_KPPLUS       78
    @equ KSC_KP1          79
    @equ KSC_KP2          80
    @equ KSC_KP3          81
    @equ KSC_KP0          82
    @equ KSC_KPDOT        83
    @equ KSC_ZENKAKUHANKAKU    85
    @equ KSC_102ND             86
    @equ KSC_F11               87
    @equ KSC_F12               88
    @equ KSC_RO                89
    @equ KSC_KATAKANA          90
    @equ KSC_HIRAGANA          91
    @equ KSC_HENKAN            92
    @equ KSC_KATAKANAHIRAGANA  93
    @equ KSC_MUHENKAN          94
    @equ KSC_KPJPCOMMA         95
    @equ KSC_KPENTER           96
    @equ KSC_RIGHTCTRL         97
    @equ KSC_KPSLASH           98
    @equ KSC_SYSRQ             99
    @equ KSC_RIGHTALT          100
    @equ KSC_LINEFEED          101
    @equ KSC_HOME              102
    @equ KSC_UP                103
    @equ KSC_PAGEUP            104
    @equ KSC_LEFT              105
    @equ KSC_RIGHT             106
    @equ KSC_END               107
    @equ KSC_DOWN              108
    @equ KSC_PAGEDOWN          109
    @equ KSC_INSERT            110
    @equ KSC_DELETE            111
    @equ KSC_MACRO             112
    @equ KSC_MUTE              113
    @equ KSC_VOLUMEDOWN        114
    @equ KSC_VOLUMEUP          115
    @equ KSC_POWER             116 ; SC System Power Down
    @equ KSC_KPEQUAL           117
    @equ KSC_KPPLUSMINUS       118
    @equ KSC_PAUSE             119
    @equ KSC_SCALE             120 ; AL Compiz Scale (Expose)
    @equ KSC_KPCOMMA           121
    @equ KSC_HANGEUL           122
    @equ KSC_HANGUEL           KSC_HANGEUL
    @equ KSC_HANJA             123
    @equ KSC_YEN               124
    @equ KSC_LEFTMETA          125
    @equ KSC_RIGHTMETA         126
    @equ KSC_COMPOSE           127
    @equ KSC_STOP              128 ; AC Stop
    @equ KSC_AGAIN             129
    @equ KSC_PROPS             130 ; AC Properties
    @equ KSC_UNDO              131 ; AC Undo
    @equ KSC_FRONT             132
    @equ KSC_COPY              133 ; AC Copy
    @equ KSC_OPEN              134 ; AC Open
    @equ KSC_PASTE             135 ; AC Paste
    @equ KSC_FIND              136 ; AC Search
    @equ KSC_CUT               137 ; AC Cut
    @equ KSC_HELP              138 ; AL Integrated Help Center
    @equ KSC_MENU              139 ; Menu (show menu)
    @equ KSC_CALC              140 ; AL Calculator
    @equ KSC_SETUP             141
    @equ KSC_SLEEP             142 ; SC System Sleep
    @equ KSC_WAKEUP            143 ; System Wake Up
    @equ KSC_FILE              144 ; AL Local Machine Browser
    @equ KSC_SENDFILE          145
    @equ KSC_DELETEFILE        146
    @equ KSC_XFER              147
    @equ KSC_PROG1             148
    @equ KSC_PROG2             149
    @equ KSC_WWW               150 ; AL Internet Browser
    @equ KSC_MSDOS             151
    @equ KSC_COFFEE            152 ; AL Terminal Lock/Screensaver
    @equ KSC_SCREENLOCK        KSC_COFFEE
    @equ KSC_ROTATE_DISPLAY    153 ; Display orientation for e.g. tablets
    @equ KSC_DIRECTION         KSC_ROTATE_DISPLAY
    @equ KSC_CYCLEWINDOWS      154
    @equ KSC_MAIL              155
    @equ KSC_BOOKMARKS         156 ; AC Bookmarks
    @equ KSC_COMPUTER          157
    @equ KSC_BACK              158 ; AC Back
    @equ KSC_FORWARD           159 ; AC Forward
    @equ KSC_CLOSECD           160
    @equ KSC_EJECTCD           161
    @equ KSC_EJECTCLOSECD      162
    @equ KSC_NEXTSONG          163
    @equ KSC_PLAYPAUSE         164
    @equ KSC_PREVIOUSSONG      165
    @equ KSC_STOPCD            166
    @equ KSC_RECORD            167
    @equ KSC_REWIND            168
    @equ KSC_PHONE             169 ; Media Select Telephone
    @equ KSC_ISO               170
    @equ KSC_CONFIG            171 ; AL Consumer Control Configuration
    @equ KSC_HOMEPAGE          172 ; AC Home
    @equ KSC_REFRESH           173 ; AC Refresh
    @equ KSC_EXIT              174 ; AC Exit
    @equ KSC_MOVE              175
    @equ KSC_EDIT              176
    @equ KSC_SCROLLUP          177
    @equ KSC_SCROLLDOWN        178
    @equ KSC_KPLEFTPAREN       179
    @equ KSC_KPRIGHTPAREN      180
    @equ KSC_NEW               181 ; AC New
    @equ KSC_REDO              182 ; AC Redo/Repeat
    @equ KSC_F13               183
    @equ KSC_F14               184
    @equ KSC_F15               185
    @equ KSC_F16               186
    @equ KSC_F17               187
    @equ KSC_F18               188
    @equ KSC_F19               189
    @equ KSC_F20               190
    @equ KSC_F21               191
    @equ KSC_F22               192
    @equ KSC_F23               193
    @equ KSC_F24               194
    @equ KSC_PLAYCD            200
    @equ KSC_PAUSECD           201
    @equ KSC_PROG3             202
    @equ KSC_PROG4             203
    @equ KSC_ALL_APPLICATIONS  204 ; AC Desktop Show All Applications
    @equ KSC_DASHBOARD         KSC_ALL_APPLICATIONS
    @equ KSC_SUSPEND           205
    @equ KSC_CLOSE             206 ; AC Close
    @equ KSC_PLAY              207
    @equ KSC_FASTFORWARD       208
    @equ KSC_BASSBOOST         209
    @equ KSC_PRINT             210 ; AC Print
    @equ KSC_HP                211
    @equ KSC_CAMERA            212
    @equ KSC_SOUND             213
    @equ KSC_QUESTION          214
    @equ KSC_EMAIL             215
    @equ KSC_CHAT              216
    @equ KSC_SEARCH            217
    @equ KSC_CONNECT           218
    @equ KSC_FINANCE           219 ; AL Checkbook/Finance
    @equ KSC_SPORT             220
    @equ KSC_SHOP              221
    @equ KSC_ALTERASE          222
    @equ KSC_CANCEL            223 ; AC Cancel
    @equ KSC_BRIGHTNESSDOWN    224
    @equ KSC_BRIGHTNESSUP      225
    @equ KSC_MEDIA             226
    @equ KSC_SWITCHVIDEOMODE   227 ; Cycle between available video outputs (Monitor/LCD/TV-out/etc)
    @equ KSC_KBDILLUMTOGGLE    228
    @equ KSC_KBDILLUMDOWN      229
    @equ KSC_KBDILLUMUP        230
    @equ KSC_SEND              231 ; AC Send
    @equ KSC_REPLY             232 ; AC Reply
    @equ KSC_FORWARDMAIL       233 ; AC Forward Msg
    @equ KSC_SAVE              234 ; AC Save
    @equ KSC_DOCUMENTS         235
    @equ KSC_BATTERY           236
    @equ KSC_BLUETOOTH         237
    @equ KSC_WLAN              238
    @equ KSC_UWB               239
    @equ KSC_UNKNOWN           240
    @equ KSC_VIDEO_NEXT        241 ; drive next video source
    @equ KSC_VIDEO_PREV        242 ; drive previous video source
    @equ KSC_BRIGHTNESS_CYCLE  243 ; brightness up, after max is min
    @equ KSC_BRIGHTNESS_AUTO   244 ; Set Auto Brightness: manual brightness control is off, rely on ambient
    @equ KSC_BRIGHTNESS_ZERO   KSC_BRIGHTNESS_AUTO
    @equ KSC_DISPLAY_OFF       245 ; display device to off state
    @equ KSC_WWAN              246 ; Wireless WAN (LTE, UMTS, GSM, etc.)
    @equ KSC_WIMAX             KSC_WWAN
    @equ KSC_RFKILL            247 ; Key that controls all radios
    @equ KSC_MICMUTE           248 ; Mute / unmute the microphone
