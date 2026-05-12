#include "RegisterScreen.h"
#include "Validator.h"
#include "Theme.h"
#include <stdexcept>
RegisterScreen::RegisterScreen(sf::Font& f, Storage<Patient>& pats)
    : font(f), patients(pats) {

    card.setSize({ 560.f, 580.f });
    card.setPosition((Theme::WIN_W - 560.f) / 2.f, (Theme::WIN_H - 580.f) / 2.f);
    card.setFillColor(Theme::PANEL);

    topBar.setSize({ 560.f, 5.f });
    topBar.setPosition(card.getPosition().x, card.getPosition().y);
    topBar.setFillColor(Theme::SUCCESS);

    float cx = card.getPosition().x;
    float cy = card.getPosition().y;

    titleText.setFont(font); titleText.setString("Patient Registration");
    titleText.setCharacterSize(Theme::FS_HEAD);
    titleText.setFillColor(Theme::TEXT_WHITE); titleText.setStyle(sf::Text::Bold);
    sf::FloatRect tb = titleText.getLocalBounds();
    titleText.setPosition(cx + (560.f - tb.width) / 2.f, cy + 20.f);

    // Helper to build a label
    auto makeLabel = [&](sf::Text& lbl, const std::string& s, float x, float y) {
        lbl.setFont(font); lbl.setString(s);
        lbl.setCharacterSize(Theme::FS_SMALL); lbl.setFillColor(Theme::TEXT_GRAY);
        lbl.setPosition(x, y);
        };

    float col1x = cx + 30.f, col2x = cx + 300.f;
    float row1y = cy + 70.f, row2y = cy + 170.f, row3y = cy + 270.f;
    float iw1 = 240.f, ih = Theme::INPUT_H;

    makeLabel(lName, "Full Name", col1x, row1y);
    makeLabel(lAge, "Age", col2x, row1y);
    makeLabel(lGender, "Gender (M/F)", col1x, row2y);
    makeLabel(lContact, "Contact (11 digits)", col2x, row2y);
    makeLabel(lPassword, "Password (min 6)", col1x, row3y);
    makeLabel(lBalance, "Initial Balance (PKR)", col2x, row3y);

    tbName = TextBox(font, "e.g. Ahmed Ali", { col1x, row1y + 22.f }, { iw1,  ih });
    tbAge = TextBox(font, "e.g. 25", { col2x, row1y + 22.f }, { 230.f,ih });
    tbGender = TextBox(font, "M or F", { col1x, row2y + 22.f }, { iw1,  ih });
    tbContact = TextBox(font, "03XXXXXXXXX", { col2x, row2y + 22.f }, { 230.f,ih });
    tbPassword = TextBox(font, "min 6 characters", { col1x, row3y + 22.f }, { iw1,  ih }, true);
    tbBalance = TextBox(font, "0", { col2x, row3y + 22.f }, { 230.f,ih });

    btnRegister = Button(font, "Register", { cx + 30.f,  cy + 490.f }, { 235.f, Theme::BTN_H }, Button::Style::SUCCESS);
    btnBack = Button(font, "Back", { cx + 295.f, cy + 490.f }, { 235.f, Theme::BTN_H }, Button::Style::GHOST);

    msgText.setFont(font); msgText.setCharacterSize(Theme::FS_SMALL);
    msgText.setPosition(cx + 30.f, cy + 548.f);
}

void RegisterScreen::tryRegister() {
    std::string name = tbName.getValue();
    std::string ageStr = tbAge.getValue();
    std::string gender = tbGender.getValue();
    std::string contact = tbContact.getValue();
    std::string pwd = tbPassword.getValue();
    std::string balStr = tbBalance.getValue();
    if (balStr.empty()) balStr = "0";

    if (name.empty()) { message = "Name cannot be empty."; success = false; return; }
    if (!Validator::isValidContact(contact)) {
        message = "Contact must be exactly 11 digits."; success = false; return;
    }
    if (!Validator::isValidPassword(pwd)) {
        message = "Password must be at least 6 characters."; success = false; return;
    }
    if (gender != "M" && gender != "F" && gender != "m" && gender != "f") {
        message = "Gender must be M or F."; success = false; return;
    }

    int age = 0;
    try { age = std::stoi(ageStr); if (age <= 0 || age >= 150) throw std::invalid_argument(""); }
    catch (...) { message = "Invalid age."; success = false; return; }

    float balance = 0.0f;
    try { balance = std::stof(balStr); if (balance < 0) throw std::invalid_argument(""); }
    catch (...) { message = "Invalid balance."; success = false; return; }

    if (gender == "m") gender = "M";
    if (gender == "f") gender = "F";

    int newId = 1;
    for (int i = 0; i < patients.size(); ++i)
        if (patients.getAt(i).getId() >= newId) newId = patients.getAt(i).getId() + 1;

    Patient p(newId, name, age, gender, contact, pwd, balance);
    patients.add(p);
    FileHandler::appendPatient(p);

    message = "Registered! Your Patient ID is: " + std::to_string(newId);
    success = true;
}

void RegisterScreen::handleEvent(const sf::Event& e, sf::RenderWindow& win) {
    tbName.handleEvent(e, win);
    tbAge.handleEvent(e, win);
    tbGender.handleEvent(e, win);
    tbContact.handleEvent(e, win);
    tbPassword.handleEvent(e, win);
    tbBalance.handleEvent(e, win);

    if (btnRegister.handleEvent(e, win)) tryRegister();
    if (btnBack.handleEvent(e, win)) { nextScreen = ScreenID::ADMIN_MENU; message.clear(); }
}

void RegisterScreen::update(float dt) {
    tbName.update(dt); tbAge.update(dt); tbGender.update(dt);
    tbContact.update(dt); tbPassword.update(dt); tbBalance.update(dt);
    msgText.setString(message);
    msgText.setFillColor(success ? Theme::SUCCESS : Theme::DANGER);
}

void RegisterScreen::draw(sf::RenderWindow& win) {
    win.draw(card); win.draw(topBar); win.draw(titleText);
    win.draw(lName); win.draw(lAge); win.draw(lGender);
    win.draw(lContact); win.draw(lPassword); win.draw(lBalance);
    tbName.draw(win); tbAge.draw(win); tbGender.draw(win);
    tbContact.draw(win); tbPassword.draw(win); tbBalance.draw(win);
    btnRegister.draw(win); btnBack.draw(win);
    win.draw(msgText);
}
