#include "DoctorScreen.h"
#include "FileHandler.h"
#include "Utils.h"
#include <sstream>
#include <iomanip>
#include <ctime>

DoctorScreen::DoctorScreen(sf::Font& f, Doctor* d, Storage<Patient>* pats,
    Storage<Appointment>* appts, Storage<Prescription>* prescs, Storage<Bill>* b)
    : font(f), doctor(d), patients(pats), appointments(appts),
    prescriptions(prescs), bills(b) {
    buildLayout();
    setupToday();
}

void DoctorScreen::buildLayout() {
    sidebar.setSize({ 220.f,(float)Theme::WIN_H }); sidebar.setPosition(0.f, 0.f);
    sidebar.setFillColor(Theme::SIDEBAR_BG);
    topBar.setSize({ 220.f,4.f }); topBar.setPosition(0.f, 0.f);
    topBar.setFillColor(Theme::ACCENT);
    contentArea.setSize({ (float)Theme::WIN_W - 220.f,(float)Theme::WIN_H });
    contentArea.setPosition(220.f, 0.f); contentArea.setFillColor(Theme::BG);
    listBg.setSize({ (float)Theme::WIN_W - 240.f,(float)Theme::WIN_H - 120.f });
    listBg.setPosition(230.f, 70.f); listBg.setFillColor(Theme::PANEL);

    headerText.setFont(font); headerText.setCharacterSize(Theme::FS_SMALL);
    headerText.setFillColor(Theme::TEXT_WHITE); headerText.setPosition(14.f, 18.f);
    headerText.setString("Dr. " + doctor->getName() + "\n" + doctor->getSpecialization());

    viewTitle.setFont(font); viewTitle.setCharacterSize(Theme::FS_HEAD);
    viewTitle.setFillColor(Theme::TEXT_WHITE); viewTitle.setStyle(sf::Text::Bold);
    viewTitle.setPosition(240.f, 20.f);

    msgText.setFont(font); msgText.setCharacterSize(Theme::FS_SMALL);
    msgText.setPosition(240.f, 680.f);

    float bx = 10.f, bw = 200.f, bh = 40.f, by = 90.f, gap = 6.f;
    btnToday = Button(font, "Today's Appointments", { bx,by }, { bw,bh }, Button::Style::PRIMARY);
    btnComplete = Button(font, "Mark Complete", { bx,by + (bh + gap) }, { bw,bh }, Button::Style::SUCCESS);
    btnNoShow = Button(font, "Mark No-Show", { bx,by + 2 * (bh + gap) }, { bw,bh }, Button::Style::DANGER);
    btnPrescription = Button(font, "Write Prescription", { bx,by + 3 * (bh + gap) }, { bw,bh }, Button::Style::GHOST);
    btnHistory = Button(font, "Patient History", { bx,by + 4 * (bh + gap) }, { bw,bh }, Button::Style::GHOST);
    btnLogout = Button(font, "Logout", { bx,by + 5 * (bh + gap) }, { bw,bh }, Button::Style::DANGER);

    lInput1.setFont(font); lInput1.setCharacterSize(Theme::FS_SMALL); lInput1.setFillColor(Theme::TEXT_GRAY);
    lInput2.setFont(font); lInput2.setCharacterSize(Theme::FS_SMALL); lInput2.setFillColor(Theme::TEXT_GRAY);
    lInput3.setFont(font); lInput3.setCharacterSize(Theme::FS_SMALL); lInput3.setFillColor(Theme::TEXT_GRAY);

    tbInput1 = TextBox(font, "", { 240.f,480.f }, { 350.f,Theme::INPUT_H });
    tbInput2 = TextBox(font, "", { 240.f,580.f }, { 600.f,Theme::INPUT_H });
    tbInput3 = TextBox(font, "", { 240.f,640.f }, { 600.f,Theme::INPUT_H });

    btnConfirm = Button(font, "Confirm", { 240.f,640.f }, { 180.f,Theme::BTN_H }, Button::Style::PRIMARY);
    btnBack2 = Button(font, "Cancel", { 440.f,640.f }, { 180.f,Theme::BTN_H }, Button::Style::GHOST);
}


void DoctorScreen::autoMarkMissed() {
    time_t t = time(nullptr);
    tm now{};
    localtime_s(&now, &t);
    int todayInt = (now.tm_year + 1900) * 10000 + (now.tm_mon + 1) * 100 + now.tm_mday;
    int curHour = now.tm_hour;
    std::string today = getTodayDate();

    bool changed = false;
    for (int i = 0; i < appointments->size(); ++i) {
        const Appointment& a = appointments->getAt(i);
        if (a.getStatus() != "pending") continue;

        int apptInt = dateToInt(a.getDate());
        bool missed = false;

        if (apptInt < todayInt) {
            missed = true;
        }
        else if (a.getDate() == today) {
            int slotHour = std::stoi(a.getTimeSlot().substr(0, 2));
            if (slotHour < curHour) missed = true;
        }

        if (missed) {
            Appointment* ap = appointments->findById(a.getAppointmentId());
            if (ap) { ap->setStatus("missed"); changed = true; }
        }
    }
    if (changed) FileHandler::saveAllAppointments(*appointments);
}

void DoctorScreen::setupToday() {
    autoMarkMissed();

    viewTitle.setString("Today's Appointments");
    listRows.clear(); scrollOffset = 0; message.clear();

    colPositions = { 238.f, 280.f, 490.f, 570.f };

    std::string today = getTodayDate();
    int idx[200]; int n = 0;
    for (int i = 0; i < appointments->size(); ++i) {
        const Appointment& a = appointments->getAt(i);
        if (a.getDoctorId() == doctor->getId() && a.getDate() == today) idx[n++] = i;
    }
    // Sort by time slot ascending
    for (int i = 1; i < n; ++i) {
        int key = idx[i], j = i - 1;
        while (j >= 0 && compareTimeSlots(appointments->getAt(idx[j]).getTimeSlot(),
            appointments->getAt(key).getTimeSlot()) > 0) {
            idx[j + 1] = idx[j]; --j;
        }
        idx[j + 1] = key;
    }
    listRows.push_back("ID|Patient|Slot|Status");
    listRows.push_back(std::string(50, '-'));

    for (int i = 0; i < n; ++i) {
        const Appointment& a = appointments->getAt(idx[i]);
        Patient* p = patients->findById(a.getPatientId());
        std::string row =
            std::to_string(a.getAppointmentId()) + "|" +
            (p ? p->getName() : "Unknown") + "|" +
            a.getTimeSlot() + "|" +
            a.getStatus();
        listRows.push_back(row);
    }
    if (n == 0) listRows.push_back("No appointments scheduled for today.");
}

void DoctorScreen::setupMarkAction(const std::string& action) {
    viewTitle.setString(action);
    setupToday();           
    viewTitle.setString(action); 
    lInput1.setString("Appointment ID");
    lInput1.setPosition(240.f, 460.f);
    tbInput1 = TextBox(font, "Enter Appointment ID", { 240.f,482.f }, { 300.f,Theme::INPUT_H });
    btnConfirm.setText("Confirm", font); btnConfirm.setPosition({ 240.f,540.f });
    btnBack2.setPosition({ 440.f,540.f });
}

void DoctorScreen::setupPrescription() {
    viewTitle.setString("Write Prescription");
    listRows.clear(); colPositions.clear(); message.clear(); scrollOffset = 0;

    lInput1.setString("Appointment ID (must be 'completed')");
    lInput1.setPosition(240.f, 80.f);
    tbInput1 = TextBox(font, "Appointment ID", { 240.f,102.f }, { 300.f,Theme::INPUT_H });

    lInput2.setString("Medicines (e.g. Paracetamol 500mg;Ibuprofen 400mg)");
    lInput2.setPosition(240.f, 162.f);
    tbInput2 = TextBox(font, "Medicine;Dosage", { 240.f,184.f }, { 700.f,Theme::INPUT_H });

    lInput3.setString("Notes");
    lInput3.setPosition(240.f, 244.f);
    tbInput3 = TextBox(font, "Doctor notes", { 240.f,266.f }, { 700.f,Theme::INPUT_H });

    btnConfirm.setText("Save Prescription", font); btnConfirm.setPosition({ 240.f,326.f });
    btnBack2.setPosition({ 450.f,326.f });
}

void DoctorScreen::setupHistory() {
    viewTitle.setString("Patient Medical History");
    listRows.clear(); colPositions.clear(); message.clear();

    lInput1.setString("Patient ID");
    lInput1.setPosition(240.f, 80.f);
    tbInput1 = TextBox(font, "Enter Patient ID", { 240.f,102.f }, { 300.f,Theme::INPUT_H });

    btnConfirm.setText("View History", font); btnConfirm.setPosition({ 240.f,162.f });
    btnBack2.setPosition({ 450.f,162.f });
}

void DoctorScreen::doMarkComplete() {
    std::string inp = tbInput1.getValue(); int id = 0;
    try { id = std::stoi(inp); }
    catch (...) { message = "Invalid ID."; msgSuccess = false; return; }
    std::string today = getTodayDate();
    Appointment* a = appointments->findById(id);
    if (!a || a->getDoctorId() != doctor->getId() || a->getStatus() != "pending" || a->getDate() != today) {
        message = "Invalid appointment ID."; msgSuccess = false; return;
    }
    a->setStatus("completed"); FileHandler::saveAllAppointments(*appointments);
    message = "Appointment marked as completed."; msgSuccess = true;
    tbInput1.clear(); setupMarkAction("Mark Appointment Complete"); viewTitle.setString("Mark Appointment Complete");
}

void DoctorScreen::doMarkNoShow() {
    std::string inp = tbInput1.getValue(); int id = 0;
    try { id = std::stoi(inp); }
    catch (...) { message = "Invalid ID."; msgSuccess = false; return; }
    std::string today = getTodayDate();
    Appointment* a = appointments->findById(id);
    if (!a || a->getDoctorId() != doctor->getId() || a->getStatus() != "pending" || a->getDate() != today) {
        message = "Invalid appointment ID."; msgSuccess = false; return;
    }
    a->setStatus("noshow"); FileHandler::saveAllAppointments(*appointments);
    for (int i = 0; i < bills->size(); ++i)
        if (bills->getAt(i).getAppointmentId() == id) {
            bills->getAt(i).setStatus("cancelled"); FileHandler::saveAllBills(*bills); break;
        }
    message = "Appointment marked as no-show."; msgSuccess = true;
    tbInput1.clear();
}

void DoctorScreen::doWritePrescription() {
    std::string inp = tbInput1.getValue(); int id = 0;
    try { id = std::stoi(inp); }
    catch (...) { message = "Invalid ID."; msgSuccess = false; return; }
    Appointment* a = appointments->findById(id);
    if (!a || a->getDoctorId() != doctor->getId() || a->getStatus() != "completed") {
        message = "Appointment not found or not completed."; msgSuccess = false; return;
    }
    for (int i = 0; i < prescriptions->size(); ++i)
        if (prescriptions->getAt(i).getAppointmentId() == id) {
            message = "Prescription already written."; msgSuccess = false; return;
        }
    std::string med = tbInput2.getValue(), notes = tbInput3.getValue();
    if (med.empty()) { message = "Enter medicines."; msgSuccess = false; return; }
    if (med.size() > 499) med = med.substr(0, 499);
    if (notes.size() > 299) notes = notes.substr(0, 299);
    int newId = 1;
    for (int i = 0; i < prescriptions->size(); ++i)
        if (prescriptions->getAt(i).getPrescriptionId() >= newId)
            newId = prescriptions->getAt(i).getPrescriptionId() + 1;
    Prescription presc(newId, id, a->getPatientId(), doctor->getId(), a->getDate(), med, notes);
    prescriptions->add(presc); FileHandler::appendPrescription(presc);
    message = "Prescription saved."; msgSuccess = true;
    tbInput1.clear(); tbInput2.clear(); tbInput3.clear();
}

void DoctorScreen::doViewHistory() {
    std::string inp = tbInput1.getValue(); int patId = 0;
    try { patId = std::stoi(inp); }
    catch (...) { message = "Access denied."; msgSuccess = false; return; }
    Patient* pat = patients->findById(patId);
    if (!pat) { message = "Access denied."; msgSuccess = false; return; }
    bool ok = false;
    for (int i = 0; i < appointments->size(); ++i) {
        const Appointment& a = appointments->getAt(i);
        if (a.getPatientId() == patId && a.getDoctorId() == doctor->getId() &&
            a.getStatus() == "completed") {
            ok = true; break;
        }
    }
    if (!ok) { message = "Access denied. No completed appointments."; msgSuccess = false; return; }

    listRows.clear(); colPositions.clear();
    int idx[200]; int n = 0;
    for (int i = 0; i < prescriptions->size(); ++i) {
        const Prescription& p = prescriptions->getAt(i);
        if (p.getPatientId() == patId && p.getDoctorId() == doctor->getId()) idx[n++] = i;
    }
    for (int i = 1; i < n; ++i) {
        int key = idx[i], j = i - 1;
        while (j >= 0 && dateToInt(prescriptions->getAt(idx[j]).getDate()) <
            dateToInt(prescriptions->getAt(key).getDate()))
        {
            idx[j + 1] = idx[j]; --j;
        }
        idx[j + 1] = key;
    }
    for (int i = 0; i < n; ++i) {
        const Prescription& p = prescriptions->getAt(idx[i]);
        listRows.push_back("Date: " + p.getDate());
        listRows.push_back("  Medicines: " + p.getMedicines());
        listRows.push_back("  Notes: " + p.getNotes());
        listRows.push_back("");
    }
    if (n == 0) listRows.push_back("No prescriptions found.");
    message = ""; scrollOffset = 0;
}

void DoctorScreen::drawList(sf::RenderWindow& win) {
    win.draw(listBg);
    float x = 238.f, lineH = 22.f;
    float y = (currentView == View::HISTORY) ? 220.f : 80.f;
    int start = scrollOffset, maxLines = (int)((Theme::WIN_H - y - 40.f) / lineH);

    sf::Text cell;
    cell.setFont(font);
    cell.setCharacterSize(14);

    for (int i = start; i < (int)listRows.size() && i < start + maxLines; ++i) {
        const std::string& line = listRows[i];

        if (i > 1 && !line.empty() && line[0] != '-') {
            sf::RectangleShape bg({ (float)Theme::WIN_W - 240.f, lineH });
            bg.setPosition(230.f, y - 2.f);
            bg.setFillColor((i % 2 == 0) ? Theme::ROW_EVEN : Theme::ROW_ODD);
            win.draw(bg);
        }

        bool hasColumns = !colPositions.empty() && (line.find('|') != std::string::npos);

        if (hasColumns) {
            std::istringstream ss(line);
            std::string token;
            int col = 0;
            while (std::getline(ss, token, '|') && col < (int)colPositions.size()) {
                cell.setString(token);
                cell.setPosition(colPositions[col], y);
                cell.setFillColor(i < 2 ? Theme::ACCENT : Theme::TEXT_WHITE);
                cell.setStyle(i < 2 ? sf::Text::Bold : sf::Text::Regular);
                win.draw(cell);
                ++col;
            }
        }
        else {
            cell.setStyle(sf::Text::Regular);
            cell.setFillColor(i == 0 ? Theme::ACCENT : Theme::TEXT_WHITE);
            cell.setString(line);
            cell.setPosition(x, y);
            win.draw(cell);
        }

        y += lineH;
    }
}

void DoctorScreen::handleEvent(const sf::Event& e, sf::RenderWindow& win) {
    if (btnToday.handleEvent(e, win)) { currentView = View::TODAY;        setupToday();                                   return; }
    if (btnComplete.handleEvent(e, win)) { currentView = View::MARK_COMPLETE; setupMarkAction("Mark Appointment Complete");  return; }
    if (btnNoShow.handleEvent(e, win)) { currentView = View::MARK_NOSHOW;   setupMarkAction("Mark Appointment No-Show");   return; }
    if (btnPrescription.handleEvent(e, win)) { currentView = View::PRESCRIPTION;  setupPrescription();                           return; }
    if (btnHistory.handleEvent(e, win)) { currentView = View::HISTORY;       setupHistory();                                return; }
    if (btnLogout.handleEvent(e, win)) { nextScreen = ScreenID::MAIN_MENU;                                                 return; }

    if (e.type == sf::Event::MouseWheelScrolled) {
        scrollOffset -= (int)e.mouseWheelScroll.delta * 2;
        if (scrollOffset < 0) scrollOffset = 0;
    }

    tbInput1.handleEvent(e, win);
    if (currentView == View::PRESCRIPTION) { tbInput2.handleEvent(e, win); tbInput3.handleEvent(e, win); }

    if (btnConfirm.handleEvent(e, win)) {
        if (currentView == View::MARK_COMPLETE) doMarkComplete();
        else if (currentView == View::MARK_NOSHOW)   doMarkNoShow();
        else if (currentView == View::PRESCRIPTION)  doWritePrescription();
        else if (currentView == View::HISTORY)       doViewHistory();
    }
    if (btnBack2.handleEvent(e, win)) { currentView = View::MENU; message.clear(); setupToday(); }
}

void DoctorScreen::update(float dt) {
    tbInput1.update(dt); tbInput2.update(dt); tbInput3.update(dt);
    msgText.setString(message);
    msgText.setFillColor(msgSuccess ? Theme::SUCCESS : Theme::DANGER);
}

void DoctorScreen::draw(sf::RenderWindow& win) {
    win.draw(contentArea); win.draw(sidebar); win.draw(topBar);
    win.draw(headerText);

    btnToday.draw(win); btnComplete.draw(win); btnNoShow.draw(win);
    btnPrescription.draw(win); btnHistory.draw(win); btnLogout.draw(win);

    win.draw(viewTitle);

    if (!listRows.empty()) drawList(win);

    if (currentView != View::TODAY && currentView != View::MENU) {
        win.draw(lInput1); tbInput1.draw(win);
        if (currentView == View::PRESCRIPTION) {
            win.draw(lInput2); tbInput2.draw(win);
            win.draw(lInput3); tbInput3.draw(win);
        }
        btnConfirm.draw(win); btnBack2.draw(win);
    }

    win.draw(msgText);
}
