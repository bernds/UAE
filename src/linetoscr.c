
static int LNAME (int spix, int dpix, int stoppos)
{
    TYPE *buf = ((TYPE *)xlinebuffer);
    int offset;

    if (bplham && bplplanecnt == 6) {
	/* HAM 6 */
	while (dpix < stoppos) {
	    TYPE d = xcolors[ham_linebuf[spix]];
	    spix += SRC_INC;
	    buf[dpix++] = d;
	    if (HDOUBLE)
		buf[dpix++] = d;
	}
    } else if (bpldualpf) {
	/* Dual playfield */
	int *lookup = bpldualpfpri ? dblpf_ind2 : dblpf_ind1;
	while (dpix < stoppos) {
	    int pixcol = pixdata.apixels[spix];
	    TYPE d = colors_for_drawing.acolors[lookup[pixcol]];
	    spix += SRC_INC;
	    buf[dpix++] = d;
	    if (HDOUBLE)
		buf[dpix++] = d;
	}
    } else if (bplehb) {
	while (dpix < stoppos) {
	    int p = pixdata.apixels[spix];
	    TYPE d = colors_for_drawing.acolors[p];
	    spix += SRC_INC;
	    if (p >= 32)
		d = xcolors[(colors_for_drawing.color_regs[p-32] >> 1) & 0x777];
	    buf[dpix++] = d;
	    if (HDOUBLE)
		buf[dpix++] = d;
	}
    } else {
	while (dpix < stoppos) {
	    TYPE d = colors_for_drawing.acolors[pixdata.apixels[spix]];
	    spix += SRC_INC;
	    buf[dpix++] = d;
	    if (HDOUBLE)
		buf[dpix++] = d;
	}
    }
    return spix;
}

#undef LNAME
#undef HDOUBLE
#undef SRC_INC
