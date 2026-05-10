#ifndef PATIENTSCREEN_H
#define PATIENTSCREEN_H
#include "Screen.h"
#include "Button.h"
#include "TextBox.h"
#include "Theme.h"
#include "Patient.h"
#include "Doctor.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "Storage.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class PatientScreen : public Screen {
public:
    enum class View {
        MENU, BOOK, CANCEL, VIEW_APPTS,
        VIEW_RECORDS, VIEW_BILLS, PAY_BILL, TOPUP
    };

private:
    sf::Font& font;
    Patient* patient;
    Storage<Patient>* patients;
    Storage<Doctor>* doctors;
    Storage<Appointment>* appointments;
    Storage<Bill>* bills;
    Storage<Prescription>* prescriptions;

    View currentView = View::MENU;

    //Layout 
    sf::RectangleShape sidebar;
    sf::RectangleShape topBar;
    sf::RectangleShape contentArea;
    sf::Text           headerName;
    sf::Text           headerBalance;

    // Sidebar buttons
    Button btnBook, btnCancel, btnViewAppts, btnRecords,
        btnBills, btnPayBill, btnTopUp, btnLogout;

    // Content area widgets (shared/reused per view)
    sf::Text            viewTitle;
    sf::Text            msgText;
    std::string         message;
    bool                msgSuccess = false;

    // Scrollable list
    std::vector<std::string> listRows;
    int                 scrollOffset = 0;
    sf::RectangleShape  listBg;

    // Form fields
    TextBox tbInput1, tbInput2, tbInput3;
    sf::Text lInput1, lInput2, lInput3;
    Button   btnConfirm, btnBack2;

    // Slot selection state (book appointment)
    int      selectedDoctorId = -1;
    std::string bookDate;
    std::vector<std::string> availableSlots;
    int      selectedSlotIdx = -1;
    std::vector<Button> slotButtons;

    //Helpers
    void buildSidebar();
    void buildContentArea();
    void refreshHeader();

    // View builders
    void setupBook();
    void setupCancel();
    void setupViewAppts();
    void setupViewRecords();
    void setupViewBills();
    void setupPayBill();
    void setupTopUp();

    // Action handlers
    void doBookStep1();    
    void doBookStep2();    
    void doConfirmBook();
    void doConfirmCancel();
    void doConfirmPayBill();
    void doConfirmTopUp();

    void drawList(sf::RenderWindow& win);
    bool isSlotTaken(int docId, const std::string& date, const std::string& slot);

    // Book appointment sub-state
    enum class BookStep { SPEC_SEARCH, SLOT_SELECT, DONE };
    BookStep bookStep = BookStep::SPEC_SEARCH;
    std::vector<int> foundDoctorIds;

public:
    PatientScreen(sf::Font& font, Patient* patient,
        Storage<Patient>* patients,
        Storage<Doctor>* doctors,
        Storage<Appointment>* appointments,
        Storage<Bill>* bills,
        Storage<Prescription>* prescriptions);

    void handleEvent(const sf::Event& e, sf::RenderWindow& win) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& win) override;
};

#endif

