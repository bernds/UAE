
static int LNAME (int spix, int dpix, int stoppos)
{
    TYPE *buf = ((TYPE *)xlinebuffer);
    uae_u8 xor_val;

    if (AGA) xor_val = (uae_u8)(dp_for_drawing->bplcon4 >> 8);
    if (bplham && bplplanecnt >= 6) {
	/* HAM 6 / HAM 8 */
	while (dpix < stoppos) {
	    TYPE d = AGA ? CONVERT_RGB (ham_linebuf[spix]) : xcolors[ham_linebuf[spix]];
	    spix += SRC_INC;
	    buf[dpix++] = d;
	    if (HDOUBLE)
		buf[dpix++] = d;
	}
    } else if (bpldualpf) {
	/* Dual playfield (AGA broken) */
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
	    if (AGA) {
		/* AGA EHB playfield */
		int ehbtmp = (p >= 32
			      ? (colors_for_drawing.color_regs_aga[(p-32)^xor_val] >> 1) & 0x7F7F7F
			      : colors_for_drawing.color_regs_aga[p]);
		d = CONVERT_RGB (ehbtmp);
	    } else {
		/* OCS/ECS EHB playfield */
		if (p >= 32)
		    d = xcolors[(colors_for_drawing.color_regs_ecs[p-32] >> 1) & 0x777];
	    }
	    buf[dpix++] = d;
	    if (HDOUBLE)
		buf[dpix++] = d;
	}
    } else {
	while (dpix < stoppos) {
	    TYPE d = (AGA ? colors_for_drawing.acolors[pixdata.apixels[spix]^xor_val]
		      : colors_for_drawing.acolors[pixdata.apixels[spix]]);
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
#undef AGA
