#include "OutputMenu.h"

OutputMenu::OutputMenu(QMenu *parent)
	: QMenu(parent)
	, speechText(new QAction("S T", this))
	, speech(new QAction("speech", this))
	, text(new QAction("text", this)){
	this->setTitle("text");

	this->addAction(speechText);
	this->addAction(speech);
	this->addAction(text);

	connect(speechText, &QAction::triggered, [this]() {
		this->setTitle(speechText->text());
		
	});
	connect(speech, &QAction::triggered, [this]() {
		this->setTitle(speech->text());
		
	});
	connect(text, &QAction::triggered, [this]() {
		this->setTitle(text->text());
	});
}