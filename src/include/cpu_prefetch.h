
STATIC_INLINE uae_u32 get_word_prefetch (int o)
{
    uae_u32 v = regs.irc;
    regs.irc = get_word (m68k_getpc() + o);
    return v;
}
STATIC_INLINE uae_u32 get_long_prefetch (int o)
{
    uae_u32 v = get_word_prefetch (o) << 16;
    v |= get_word_prefetch (o + 2);
    return v;
}

