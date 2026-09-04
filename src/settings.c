#include "settings.h"

#include "state.h"
#include "ui.h"

void settings_run(void)
{
    if (ui_confirm("Reset High Score?")) {
        state_reset_high_score();
        ui_message("High Score Reset!");
    }
}

