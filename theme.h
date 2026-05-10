#ifndef THEME_H
#define THEME_H
#include <SFML/Graphics.hpp>
#include <string>
namespace Theme {
    // Window
    constexpr unsigned int WIN_W = 1280;
    constexpr unsigned int WIN_H = 720;

    // Font sizes
    constexpr unsigned int FS_TITLE = 32;
    constexpr unsigned int FS_HEAD = 22;
    constexpr unsigned int FS_BODY = 18;
    constexpr unsigned int FS_SMALL = 15;

    // Colors
    const sf::Color BG(15, 17, 35);   // deep navy background
    const sf::Color PANEL(20, 65, 85);   // dark teal panel
    const sf::Color ACCENT(70, 140, 230);   // blue accent
    const sf::Color ACCENT_DARK(45, 95, 175);   // darker blue (hover)
    const sf::Color SUCCESS(50, 200, 120);   // green
    const sf::Color DANGER(220, 70, 70);   // red
    const sf::Color WARNING(230, 175, 50);   // yellow
    const sf::Color TEXT_WHITE(240, 240, 255);  // main text
    const sf::Color TEXT_GRAY(150, 155, 175);  // secondary text
    const sf::Color TEXT_DARK(80, 85, 110);  // muted text
    const sf::Color INPUT_BG(25, 75, 98);  // text box background
    const sf::Color INPUT_ACTIVE(30, 88, 112);  // active text box
    const sf::Color BORDER(60, 65, 95);  // border color
    const sf::Color ROW_EVEN(15, 55, 72);  // table row even
    const sf::Color ROW_ODD(18, 60, 78);  // table row odd
    const sf::Color ROW_HOVER(40, 45, 80);  // table row hover
    const sf::Color SIDEBAR_BG(12, 48, 65);  // sidebar

    // Standard padding / margin
    constexpr float PAD = 16.f;
    constexpr float BTN_H = 44.f;
    constexpr float INPUT_H = 44.f;
    constexpr float ROW_H = 36.f;
}
#endif

