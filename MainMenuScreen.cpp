#include "MainMenuScreen.h"
#include "Theme.h"

MainMenuScreen::MainMenuScreen(sf::Font& f) : font(f) {
    card.setSize({ 440.f, 460.f });
    card.setPosition((Theme::WIN_W - 440.f) / 2.f, (Theme::WIN_H - 460.f) / 2.f);
    card.setFillColor(Theme::PANEL);

    // Logo bar at top of card
    logo.setSize({ 440.f, 6.f });
    logo.setPosition(card.getPosition().x, card.getPosition().y);
    logo.setFillColor(Theme::ACCENT);

    float cx = card.getPosition().x;
    float cy = card.getPosition().y;

    // Title
    title.setFont(font);
    title.setString("MediCore");
    title.setCharacterSize(Theme::FS_TITLE);
    title.setFillColor(Theme::ACCENT);
    title.setStyle(sf::Text::Bold);
    sf::FloatRect tb = title.getLocalBounds();
    title.setPosition(cx + (440.f - tb.width) / 2.f, cy + 26.f);

    // Subtitle
    subtitle.setFont(font);
    subtitle.setString("Hospital Management System");
    subtitle.setCharacterSize(Theme::FS_SMALL);
    subtitle.setFillColor(Theme::TEXT_GRAY);
    sf::FloatRect sb = subtitle.getLocalBounds();
    subtitle.setPosition(cx + (440.f - sb.width) / 2.f, cy + 72.f);

    // Buttons — stacked vertically
    float bx = cx + 40.f;
    float bw = 360.f;
    float bh = Theme::BTN_H;
    float by = cy + 120.f;
    float gap = 16.f;

    btnPatient  = Button(font, "Login as Patient",{ bx, by }, { bw, bh }, Button::Style::PRIMARY);
    btnDoctor   = Button(font, "Login as Doctor",{ bx, by + (bh + gap) }, { bw, bh }, Button::Style::GHOST);
    btnAdmin    = Button(font, "Login as Admin", { bx, by + 2 * (bh + gap) }, { bw, bh }, Button::Style::GHOST);
    btnExit     = Button(font, "Exit", { bx, by + 3 * (bh + gap) }, { bw, bh }, Button::Style::DANGER);
}
void MainMenuScreen::handleEvent(const sf::Event& e, sf::RenderWindow& win) {
    if (btnPatient.handleEvent(e, win)) { nextScreen = ScreenID::LOGIN_PATIENT; return; }
    if (btnDoctor.handleEvent(e, win)){ nextScreen = ScreenID::LOGIN_DOCTOR;  return; }
    if (btnAdmin.handleEvent(e, win)){ nextScreen = ScreenID::LOGIN_ADMIN;   return; }
    if (btnExit.handleEvent(e, win)){ win.close(); }
}
void MainMenuScreen::update(float /*dt*/) {}
void MainMenuScreen::draw(sf::RenderWindow& win) {
    win.draw(card);
    win.draw(logo);
    win.draw(title);
    win.draw(subtitle);
    btnPatient.draw(win);
    btnDoctor.draw(win);
    btnAdmin.draw(win);
    btnExit.draw(win);
}
