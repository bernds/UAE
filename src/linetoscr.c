
static int LNAME (int spix, int dpix, int stoppos)
{
    TYPE *buf = ((TYPE *)xlinebuffer);
    uae_u8 xor_val;

    if (AGA) xor_val = (uae_u8)(dp_for_drawing->bplcon4 >> 8);
    if (dp_for_drawing->ham_seen) {
	/* HAM 6 / HAM 8 */
	while (dpix < stoppos) {
	    TYPE d = AGA ? CONVERT_RGB (ham_linebuf[spix]) : xcolors[ham_linebuf[spix]];
	    spix += SRC_INC;
	    buf[dpix++] = d;
	    if (HDOUBLE)
		buf[dpix++] = d;
	}
    } else if (bpldualpf) {
	if (AGA) {
	    /* AGA Dual playfield */
	    int *lookup = bpldualpfpri ? dblpf_ind2_aga : dblpf_ind1_aga;
	    int *lookup_no = bpldualpfpri ? dblpf_2nd2 : dblpf_2nd1;
	    while (dpix < stoppos) {
		int pixcol = pixdata.apixels[spix];
		TYPE d;
		if (spriteagadpfpixels[spix]) {
		    d = colors_for_drawing.acolors[spriteagadpfpixels[spix]];
		    spriteagadpfpixels[spix]=0;
		} else {
		    int val = lookup[pixcol];
		    if (lookup_no[pixcol] == 2)  val += dblpfofs[bpldualpf2of];
		    /* val ^= xor; ??? */
		    d = colors_for_drawing.acolors[val];
		}
		spix += SRC_INC;
		buf[dpix++] = d;
		if (HDOUBLE)
		    buf[dpix++] = d;
	    }
	} else {
	    /* OCS/ECS Dual playfield  */
	    int *lookup = bpldualpfpri ? dblpf_ind2 : dblpf_ind1;
	    while (dpix < stoppos) {
		int pixcol = pixdata.apixels[spix];
		TYPE d = colors_for_drawing.acolors[lookup[pixcol]];
		spix += SRC_INC;
		buf[dpix++] = d;
		if (HDOUBLE)
		    buf[dpix++] = d;
	    }
	}
    } else if (bplehb) {
	while (dpix < stoppos) {
	    int p = pixdata.apixels[spix];
	    TYPE d = colors_for_drawing.acolors[p];
	    spix += SRC_INC;
	    if (AGA) {
		/* AGA EHB playfield */
		if (p>= 32 && p < 64) /* FIXME: what about sprite colors between 32 and 64? */
		    d = (colors_for_drawing.color_regs_aga[(p-32)^xor_val] >> 1) & 0x7F7F7F;
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
