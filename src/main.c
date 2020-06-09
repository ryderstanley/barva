#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/pulseaudio.h>
#include <signal.h>

#include "input.h"
#include "output.h"
#include "queue.h"
#include "opts.h"

struct color initial_bg;
void restore_bg(int sig);

int main(int argc, char* argv[]) {
    struct opts opts = parse_opts(argc, argv);

    initial_bg = opts.bg;
    signal(SIGINT, restore_bg);
    signal(SIGTERM, restore_bg);

    pa_simple *s = get_pa_simple(opts.source);

    struct queue queue = init_queue(opts.inertia);

    for (;;) {
        float buffer[SAMPLE_CHUNK];
        get_samples(s, buffer);
        queue_put(&queue, buffer, SAMPLE_CHUNK);
        set_bg(color_in_between(opts.bg, opts.target, queue_mean(&queue)));
    }
}

void restore_bg(int sig) {
    set_bg(initial_bg);
    exit(0);
}
