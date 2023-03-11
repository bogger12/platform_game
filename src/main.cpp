#include "GameSpace.cpp"

// ram tracking includes
#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>

const char* trackram(vm_size_t page_size, mach_port_t mach_port, mach_msg_type_number_t count, vm_statistics64_data_t vm_stats) {
    
    const char* out;
    
    if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
        KERN_SUCCESS == host_statistics64(mach_port, HOST_VM_INFO,
                                        (host_info64_t)&vm_stats, &count))
    {
        long long free_memory = (int64_t)vm_stats.free_count * (int64_t)page_size;

        long long used_memory = ((int64_t)vm_stats.active_count +
                                 (int64_t)vm_stats.inactive_count +
                                 (int64_t)vm_stats.wire_count) *  (int64_t)page_size;
        out = formatstring("free memory: %lld\nused memory: %lld", free_memory, used_memory);
    }

    return out;
}
// ram tracking over

GameSpace *gamespace = nullptr;

int main(int argc, const char * argv[]) {

    // ram tracking ---
    vm_size_t page_size;
    mach_port_t mach_port;
    mach_msg_type_number_t count;
    vm_statistics64_data_t vm_stats;
    mach_port = mach_host_self();
    count = sizeof(vm_stats) / sizeof(natural_t);

    const int FPS = 60;
    const int frameDelay = 1000/FPS;

    Uint32 frameStart;
    int frameTime;

    gamespace = new GameSpace();

    gamespace->init("pissfart", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1100, 750, false);

    while (gamespace->running()) {

        frameStart = SDL_GetTicks();

        gamespace->handleEvents();
        gamespace->update();
        gamespace->render();

        frameTime = SDL_GetTicks() - frameStart;

        std::string frametimetext = "frametime: " + std::to_string(frameTime);
        //std::cout << frameTime << std::endl;
        GameSpace::textarray[frametimetxt]->UpdateText(frametimetext.c_str(), false);

        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
        GameSpace::textarray[ramtrackingtxt]->UpdateText(trackram(page_size, mach_port, count, vm_stats), false);


    }

    gamespace->destroyWindow();

    return 0;
}