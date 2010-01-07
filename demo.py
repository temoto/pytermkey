import termkey


result_str_map = {
    termkey.RES_NONE: 'None',
    termkey.RES_KEY: 'Key',
    termkey.RES_EOF: 'EOF',
    termkey.RES_AGAIN: 'AGAIN',
}


tk = termkey.TermKey(0, 0)
tk.flags = termkey.FLAG_RAW
tk.waittime = tk.waittime

keystr = ''
while keystr != 'q':
    result, key = tk.waitkey()
    keystr = str(key)
    print "*** result", result_str_map[result]
    print "    keystr", repr(keystr)
#    print "    utf8", key.utf8

tk.close()
