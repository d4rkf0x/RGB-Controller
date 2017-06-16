#include "irc.h"

irc::irc(QObject *parent) : QObject(parent)
{
 
}

void irc::setup(QString srv, int p, QString c, QString n)
{
	server = srv;
	port = p;
	channel = c;
	name = n;
	qDebug() << "irc::setup()";
	/* setup socket, connect signal/slot */
	socket = new QTcpSocket(this);
	connect(socket, SIGNAL(readyRead()), this, SLOT(read()));
	contest = new QTimer(this);
	connect(contest, SIGNAL(timeout()), this, SLOT(testcon()));
	/* connect */
	con();
}

void irc::read()
{
	QString line = socket->readLine();
	if (line.contains("PING :"))
	{
		QString sline = "PONG :" + line.split(":")[1];
		socket->write(sline.toLatin1());
	}
	QString tmp = name + " MODE " + name + " :+";
	if (line.contains(tmp))
	{
		buf = "JOIN " + channel + "\r\n";
		socket->write(buf.toUtf8());
	}
	if (line.contains("!")) /* most likely a message to handle */
		handle(line);
	if(socket->canReadLine())
		read();
}

void irc::con()
{
	socket->connectToHost(server, port);
	buf = "NICK " + name + "\r\n";
	socket->write(buf.toUtf8());
	buf = "USER " + name + " 8 * :" + name + "\r\n";
	socket->write(buf.toUtf8());
	contest->start(60000);
}

void irc::discon()
{
	socket->write("QUIT :elegant quit \r\n");
	socket->close();
	contest->stop();
}

void irc::handle(QString str)
{
	/* :Scruffy!Scruff@i.am.huskehhh.com PRIVMSG #csgo :You a vim faggot? */
	if (str.contains("PRIVMSG"))
	{

		QString usr = str.split("!")[0].replace(":", "");
		QString msg = str.split("PRIVMSG")[1].split(":")[1];
		if (usr == "daniel_j")
		{
			if (msg.contains("!send"))
			{
				
				msg.remove(QRegExp("[\\n\\t\\r]"));
				msg.replace("!send ", "");
				qDebug() << "message for me";
				if (msg.contains(";"))
				{
					QStringList tmp = msg.split(";");
					for (int x = 0; x < tmp.size(); x++)
					{
						emit sendcmd(tmp.at(x));

					}
				}
				else
				{
					emit sendcmd(msg);
				}
			}
		}
	}
}

void irc::sendmsg(QString msg)
{
	buf = "PRIVMSG " + channel + " :" + msg + " \r\n";
	socket->write(buf.toUtf8());
}

void irc::testcon()
{
	qDebug() << socket->state();
	if (socket->state() == QAbstractSocket::UnconnectedState)
	{
		qDebug() << "reconnect needed";
		discon();
		name = name + "_";
		con();
	}
}
