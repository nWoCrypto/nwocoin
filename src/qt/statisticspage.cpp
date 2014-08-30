#include "statisticspage.h"
#include "ui_statisticspage.h"
#include "main.h"
#include "wallet.h"
#include "init.h"
#include "base58.h"
#include "clientmodel.h"
#include "bitcoinrpc.h"
#include <sstream>
#include <string>

using namespace json_spirit;

StatisticsPage::StatisticsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticsPage)
{
    ui->setupUi(this);
    
    setFixedSize(400, 420);
    
    connect(ui->startButton, SIGNAL(pressed()), this, SLOT(updateStatistics()));

	powdiffPrevious = -1;
	posdiffPrevious = -1;
	powhashPrevious = -1;
	connectionPrevious = -1;
}


void StatisticsPage::updateStatistics()
{
	int nHeight = pindexBest->nHeight;
    int64_t powvalue = GetProofOfWorkReward(nHeight, 0, pindexBest);
	int64_t powvalue0 = powvalue / COIN;
	QString subsidy = QString::number((int)powvalue0) + " NWOs per block";
    double powdiff0 = GetDifficulty();
    double posdiff0 = GetDifficulty(GetLastBlockIndex(pindexBest, true));
    double powhash0 = GetPoWMHashPS();
    int peers0 = this->model->getNumConnections();
    int64_t coinsupply0 = ((pindexBest->nMoneySupply)/100000000) - 200000;
    uint64_t nMinWeight = 0, nMaxWeight = 0, nWeight = 0;
    pwalletMain->GetStakeWeight(*pwalletMain, nMinWeight, nMaxWeight, nWeight);
    uint64_t nNetworkWeight = GetPoSKernelPS();

    QString height = QString::number(nHeight);
    QString powdiff = QString::number(powdiff0, 'f', 6);
    QString posdiff = QString::number(posdiff0, 'f', 6);
    QString powhash = QString::number(powhash0, 'f', 3);
    QString stakemin = QString::number(nMinWeight);
    QString stakemax = QString::number(nNetworkWeight);
    QString phase = "";
    QString blockDate = model->getLastBlockDate().toString();
    QString peers = QString::number(peers0);
    QString coinsupply = QLocale(QLocale::English).toString(coinsupply0);

	int nPoWHeight = GetPowHeight(pindexBest) + 1;
    if (nPoWHeight < 86400)
    {
        phase = "X13 PoW and PoS";
    }
    else
    {
        phase = "Pure PoS";
    }

    ui->heightBox->setText("<b><font color=\"lightblue\">" + height + "</font></b>");
    ui->rewardBox->setText("<b><font color=\"lightblue\">" + subsidy + "</font></b>");

    if(powdiff0 >= powdiffPrevious)
    {
        ui->diffBox->setText("<b><font color=\"lightblue\">" + powdiff + "</font></b>");        
    } 
	else
	{
        ui->diffBox->setText("<b><font color=\"lightred\">" + powdiff + "</font></b>");
    } 

    if(posdiff0 >= posdiffPrevious)
    {
        ui->diffBox->setText("<b><font color=\"lightblue\">" + posdiff + "</font></b>");        
    } 
	else
	{
        ui->diffBox->setText("<b><font color=\"lightred\">" + posdiff + "</font></b>");
    } 

    if(powhash0 >= powhashPrevious)
    {
        ui->pawrateBox->setText("<b><font color=\"lightblue\">" + powhash + " MH/s</font></b>");
    } 
	else 
	{
        ui->pawrateBox->setText("<b><font color=\"lightred\">" + powhash + " MH/s</font></b>");
    } 

	ui->minBox->setText("<b><font color=\"lightblue\">" + stakemin + "</font></b>");
	ui->maxBox->setText("<b><font color=\"lightblue\">" + stakemax + "</font></b>");
	ui->cBox->setText("<b><font color=\"lightblue\">" + phase + "</font></b>");
	ui->localBox->setText("<b><font color=\"lightblue\">" + blockDate + "</font></b>");

    if(peers0 >= connectionPrevious)
    {
        ui->connectionBox->setText("<b><font color=\"lightblue\">" + peers + "</font></b>");   
    } 
	else 
	{
        ui->connectionBox->setText("<b><font color=\"lightred\">" + peers + "</font></b>");        
    } 

    ui->volumeBox->setText("<b><font color=\"lightblue\">" + coinsupply + " NWOs (excluding service coins)" + "</font></b>");

    updatePrevious(powdiff0, posdiff0, powhash0, peers0);
}

void StatisticsPage::updatePrevious(double powdiff, double posdiff, double powhash, int connection)
{
	powdiffPrevious = powdiff;
	posdiffPrevious = posdiff;
	powhashPrevious = powhash;
	connectionPrevious = connection;
}

void StatisticsPage::setModel(ClientModel *model)
{
    updateStatistics();
    this->model = model;
}


StatisticsPage::~StatisticsPage()
{
    delete ui;
}
