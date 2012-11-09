defineReplace(cleanPath) {
    win32:1 ~= s|\\\\|/|g
    contains(1, ^/.*):pfx = /
    else:pfx =
    segs = $$split(1, /)
    out =
    for(seg, segs) {
	equals(seg, ..):out = $$member(out, 0, -2)
	else:!equals(seg, .):out += $$seg
    }
    return($$join(out, /, $$pfx))
}

sub_dir = $$_PRO_FILE_PWD_
sub_dir ~= s,^$$re_escape($$PWD),,

VREEN_BUILD_TREE = $$cleanPath($$OUT_PWD)
VREEN_BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,
VREEN_INCLUDE_DIR = $$VREEN_BUILD_TREE/include/vreen
