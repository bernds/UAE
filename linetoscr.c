
static int LNAME (int spix, int dpix, int stoppos)
{
    TYPE *buf = ((TYPE *)xlinebuffer);
    uae_u8 xor_val;

#ifdef AGA
    if (AGAC)
	xor_val = (uae_u8)(dp_for_drawing->bplcon4 >> 8);
#endif
    if (dp_for_drawing->ham_seen) {
	/* HAM 6 / HAM 8 */
	while (dpix < stoppos) {
	    TYPE d = (TYPE)(AGAC ? CONVERT_RGB (ham_linebuf[spix]) : xcolors[ham_linebuf[spix]]);
	    spix += SRC_INC;
	    buf[dpix++] = d;
	    if (HDOUBLE)
		buf[dpix++] = d;
	}
    } else if (bpldualpf) {
	if (AGAC) {
	    /* AGA Dual playfield */
	    int *lookup = bpldualpfpri ? dblpf_ind2_aga : dblpf_ind1_aga;
	    int *lookup_no = bpldualpfpri ? dblpf_2nd2 : dblpf_2nd1;
	    while (dpix < stoppos) {
		int pixcol = pixdata.apixels[spix];
		TYPE d;
		if (spriteagadpfpixels[spix]) {
		    d = (TYPE)colors_for_drawing.acolors[spriteagadpfpixels[spix]];
		    spriteagadpfpixels[spix]=0;
		} else {
		    int val = lookup[pixcol];
		    if (lookup_no[pixcol] == 2)  val += dblpfofs[bpldualpf2of];
		    val ^= xor_val;
		    d = (TYPE)colors_for_drawing.acolors[val];
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
		TYPE d = (TYPE)colors_for_drawing.acolors[lookup[pixcol]];
		spix += SRC_INC;
		buf[dpix++] = d;
		if (HDOUBLE)
		    buf[dpix++] = d;
	    }
	}
    } else if (bplehb) {
	while (dpix < stoppos) {
	    int p = AGAC ? pixdata.apixels[spix] ^ xor_val : pixdata.apixels[spix];
	    TYPE d = (TYPE)colors_for_drawing.acolors[p];
	    spix += SRC_INC;
#ifdef AGA
	    if (AGAC) {
		/* AGA EHB playfield */
		if (p>= 32 && p < 64) {
		    int c = (colors_for_drawing.color_regs_aga[p-32] >> 1) & 0x7F7F7F;
		    d = CONVERT_RGB (c);
		}
	    } else {
#endif
		/* OCS/ECS EHB playfield */
		if (p >= 32)
		    d = (TYPE)xcolors[(colors_for_drawing.color_regs_ecs[p-32] >> 1) & 0x777];
#ifdef AGA
	    }
#endif
	    buf[dpix++] = d;
	    if (HDOUBLE)
		buf[dpix++] = d;
	}
    } else {
	while (dpix < stoppos) {
	    TYPE d = (TYPE)(AGAC ? colors_for_drawing.acolors[pixdata.apixels[spix] ^ xor_val]
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
#undef AGAC

