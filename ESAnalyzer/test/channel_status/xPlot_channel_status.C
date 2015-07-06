void drawBorders( int plane, float sx, float sy ) {
	const int ixES[346] = {
		1, 13,  5,  5,  5,  7,  7,  7,  7,  9,  9,  9, 11, 11, 13, 13, 13, 15, 15, 15, 
		15, 15, 15, 19, 19, 19, 21, 21, 21, 23, 23, 23, 25, 25, 25, 27, 27, 27, 27, 29, 
		29, 29, 29, 31, 31, 31, 31, 31, 31, 33, 33, 33, 35, 35, 39, 27, 35, 35, 35, 33, 
		33, 33, 33, 31, 31, 31, 29, 29, 27, 27, 27, 25, 25, 25, 25, 25, 25, 21, 21, 21, 
		19, 19, 19, 17, 17, 17, 15, 15, 15, 13, 13, 13, 13, 11, 11, 11, 11,  9,  9,  9,
		9,  9,  9,  7,  7,  7,  5,  5, 

		1,  1,  1,  2,  2,  2,  2,  3,  3,  3,  3,  4,  4,  4,  4,  5,  5,  5,  5,  6, 
		6,  6,  6,  7,  7,  7,  7,  9,  9,  9,  9, 10, 10, 10, 10, 13, 13, 13, 13, 15, 
		15, 15, 15, 25, 25, 25, 25, 27, 27, 27, 27, 30, 30, 30, 30, 31, 31, 31, 31, 33, 
		33, 33, 33, 34, 34, 34, 34, 35, 35, 35, 35, 36, 36, 36, 36, 37, 37, 37, 37, 38, 
		38, 38, 38, 39, 39, 39, 39, 38, 38, 38, 38, 37, 37, 37, 37, 36, 36, 36, 36, 35, 
		35, 35, 35, 34, 34, 34, 34, 33, 33, 33, 33, 31, 31, 31, 31, 30, 30, 30, 30, 27, 
		27, 27, 27, 25, 25, 25, 25, 15, 15, 15, 15, 13, 13, 13, 13, 10, 10, 10, 10,  9, 
		9,  9,  9,  7,  7,  7,  7,  6,  6,  6,  6,  5,  5,  5,  5,  4,  4,  4,  4,  3, 
		3,  3,  3,  2,  2,  2,  2,  1,  1,  1,

		13, 13, 13, 14, 14, 14, 14, 15, 15, 16, 16, 16, 16, 18, 18, 18, 18, 22, 22, 22, 
		22, 24, 24, 24, 24, 25, 25, 25, 25, 26, 26, 26, 26, 27, 27, 27, 27, 26, 26, 26, 
		26, 25, 25, 24, 24, 24, 24, 22, 22, 22, 22, 18, 18, 18, 18, 16, 16, 16, 16, 15, 
		15, 15, 15, 14, 14, 14, 14, 13 
	};

	const int  iyES[346] = {
		20, 20, 20, 30, 30, 30, 30, 36, 32, 32, 32, 20, 20, 38, 39, 26, 26, 26, 26, 24, 
		40, 30, 30, 30, 40, 27, 40, 35, 35, 35, 35, 26, 24, 40, 26, 26, 26, 33, 33, 33, 
		38, 24, 24, 24, 20, 24, 24, 28, 28, 28, 28, 36, 20, 33, 20, 20, 20, 10, 10, 10, 
		10,  4,  8,  8,  8, 20, 20,  2,  1, 14, 14, 14, 14, 16,  0, 10, 10, 10, 13,  0, 
		0,  5,  5,  5,  5, 14, 16,  0, 14, 14, 14,  7,  7,  7,  2, 16, 16, 16, 20, 16, 
		16, 12, 12, 12, 12,  4,  7, 20, 

		20, 26, 26, 26, 26, 28, 28, 28, 28, 31, 31, 31, 31, 32, 32, 32, 32, 33, 33, 33, 
		33, 34, 34, 34, 34, 36, 36, 36, 36, 37, 37, 37, 37, 38, 38, 38, 38, 39, 39, 39, 
		39, 40, 40, 40, 40, 39, 39, 39, 39, 38, 38, 38, 38, 37, 37, 37, 37, 36, 36, 36, 
		36, 34, 34, 34, 34, 33, 33, 33, 33, 32, 32, 32, 32, 31, 31, 31, 31, 28, 28, 28, 
		28, 26, 26, 26, 26, 14, 14, 14, 14, 12, 12, 12, 12,  9,  9,  9,  9,  8,  8,  8, 
		8,  7,  7,  7,  7,  6,  6,  6,  6,  4,  4,  4,  4,  3,  3,  3,  3,  2,  2,  2, 
		2,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  2,  2,  2,  2,  3,  3,  3, 
		3,  4,  4,  4,  4,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  8,  9,  9,  9, 
		9, 12, 12, 12, 12, 14, 14, 14, 14, 20,  

		18, 22, 22, 22, 22, 24, 24, 24, 25, 25, 25, 26, 26, 26, 26, 27, 27, 27, 27, 26, 
		26, 26, 26, 25, 25, 25, 25, 24, 24, 24, 24, 22, 22, 22, 22, 18, 18, 18, 18, 16, 
		16, 16, 15, 15, 15, 14, 14, 14, 14, 13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 
		15, 16, 16, 16, 16, 18, 18, 18
	};

	const int  lsES[54] = { // line style
		1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 
		1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 2, 2, 1, 
		2, 2, 2, 1, 2, 2, 2, 1, 1, 1, 2, 2, 2, 2
	};

	const int  lwES[54] = { // line width
		2, 2, 2, 2, 1, 1, 1, 2, 2, 2, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 
		2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 2, 2, 2, 1, 1, 2, 
		1, 1, 1, 2, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1
	};

	TLine l;

	switch (plane){

	case 1:	//ES+F
		for ( int i=0; i<346; i=i+2) {
			if (i<54*2) {
				l.SetLineStyle(lsES[i/2]);
				l.SetLineWidth(lwES[i/2]);
			} else {
				l.SetLineStyle(3);
				l.SetLineWidth(2);
			} 
			//cout<<ixES[i]+sx<<" "<<iyES[i]+sy<<" "<<ixES[i+1]+sx<<" "<<iyES[i+1]+sy<<endl;
			l.DrawLine(ixES[i]+sx, iyES[i]+sy, ixES[i+1]+sx, iyES[i+1]+sy);
		}
		break;

	case 2:   //ES-F
		for ( int i=0; i<346; i=i+2) {
			if (i<54*2) {
				l.SetLineStyle(lsES[i/2]);
				l.SetLineWidth(lwES[i/2]);
			} else {
				l.SetLineStyle(3);
				l.SetLineWidth(2);
			} 
			l.DrawLine(40-ixES[i]+sx, iyES[i]+sy, 40-ixES[i+1]+sx, iyES[i+1]+sy);
		}
		break;

	case 3:    //ES+R
		for ( int i=0; i<346; i=i+2) {
			if (i<54*2) {
				l.SetLineStyle(lsES[i/2]);
				l.SetLineWidth(lwES[i/2]);
			} else {
				l.SetLineStyle(3);
				l.SetLineWidth(2);
			} 
			l.DrawLine(40-iyES[i]+sx, ixES[i]+sy, 40-iyES[i+1]+sx, ixES[i+1]+sy);
		}
		break;

	case 4:    //ES-R
		for ( int i=0; i<346; i=i+2) {
			if (i<54*2) {
				l.SetLineStyle(lsES[i/2]);
				l.SetLineWidth(lwES[i/2]);
			} else {
				l.SetLineStyle(3);
				l.SetLineWidth(2);
			} 
			l.DrawLine(iyES[i]+sx, ixES[i]+sy, iyES[i+1]+sx, ixES[i+1]+sy);
		}
		break;

	default:
		break;

	}
}

void xPlot_channel_status() {
	gStyle->SetOptStat(0);

	TH2F *CS_PF = new TH2F("CS_PF","Channel Status ES+F",40,1,41,40,1,41);
	TH2F *CS_NF = new TH2F("CS_NF","Channel Status ES-F",40,1,41,40,1,41);
	TH2F *CS_PR = new TH2F("CS_PR","Channel Status ES+R",40,1,41,40,1,41);
	TH2F *CS_NR = new TH2F("CS_NR","Channel Status ES-R",40,1,41,40,1,41);
	
	int z[4251],p[4251],x[4251],y[4251];
	Char_t hname[500];
	ifstream in;
	in.open("channelStatus_20150704.txt");
	int max; in >> max;
	for (int n=0;n<max;n++){
		in >>z[n]>>p[n]>>x[n]>>y[n];
		cout <<z[n]<<" "<<p[n]<<" "<<x[n]<<" "<<y[n]<<endl;
		if (!in.good()) break;
		if (z[n]==1 && p[n]==1) CS_PF->Fill(x[n],y[n],1);
		if (z[n]==-1 && p[n]==1) CS_NF->Fill(x[n],y[n],1);
		if (z[n]==1 && p[n]==2) CS_PR->Fill(x[n],y[n],1);
		if (z[n]==-1 && p[n]==2) CS_NR->Fill(x[n],y[n],1);
	}
	in.close();

	TCanvas *c = new TCanvas("c","channelStatus_20150704",1000,1200);
	c->SetTitle("channelStatus_20150704");
	//c->Divide(2,2);
	//TPad * padTitle = new TPad("padTitle","padTitle",0.02, 0.02,0.98,0.098);
	TPaveLabel *pl = new TPaveLabel(0.02, 0.98,0.98,0.902,"channelStatus_20150704","");
	//pl->SetFillColor(18);
	//pl->SetTextFont(32);
	//pl->SetTextColor(49);
	pl->Draw();
	TPad * pad1 = new TPad("pad1","pad1",0.5	,0.45	,0		,0.9	);
	TPad * pad2 = new TPad("pad2","pad2",1		,0.45	,0.5	,0.9	);
	TPad * pad3 = new TPad("pad3","pad3",0.5	,0		,0		,0.45	);
	TPad * pad4 = new TPad("pad4","pad4",1		,0		,0.5	,0.45	);
	pad1->Draw();
	pad2->Draw();
	pad3->Draw();
	pad4->Draw();
	pad1->cd();
	CS_PF->GetXaxis()->SetTitle("X");
	CS_PF->GetYaxis()->SetTitle("Y");
	CS_PF->Draw("col");
	drawBorders(1,1,1);/*
	TLatex text;
	text.SetNDC();
	text.SetTextSize(0.04);
	text.SetTextFont(42);
	text.SetTextColor(kBlue);
	text.SetTextColor(1);
	text.DrawLatex(0.2, 0.85,"channelStatus_20150704");*/
	pad2->cd();
	CS_NF->GetXaxis()->SetTitle("X");
	CS_NF->GetYaxis()->SetTitle("Y");
	CS_NF->Draw("col");
	drawBorders(2,1,1); 
	pad3->cd();
	CS_PR->GetXaxis()->SetTitle("X");
	CS_PR->GetYaxis()->SetTitle("Y");
	CS_PR->Draw("col");
	drawBorders(3,1,1); 
	pad4->cd();
	CS_NR->GetXaxis()->SetTitle("X");
	CS_NR->GetYaxis()->SetTitle("Y");
	CS_NR->Draw("col");
	drawBorders( 4, 1, 1);
	c->SaveAs("ES_channel_status.png");
}
