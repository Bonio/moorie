#include "Mailboxes.h"
#include <iostream>

std::string getMailboxName(int id) {
	switch (id)
	{
//      case Walla: return "walla.com";
		case Yahoo: return "yahoo.com";
		case GMail: return "gmail.com";
		case O2:    return "o2.pl";
		case WP:    return "wp.pl";
		case OiComBr: return "oi.com.br";
		case MyNet: return "mynet.com";
//      case GMX:   return "gmx.net";
//      case M2X:   return "m2x.net";
//      case Lycos: return "lycos.com";
//      case PortugalMail: return "portugalmail";
//      case Rediff: return "rediff";
//      case CentrumSk: return "centrum.sk";
//      case CentrumCz: return "centrum.cz";
//      case BolComBr:  return "bol.com.br";
//      case PostSk: return "post.sk";
		case AOLPL: return "aol.pl";
		case AOLDE: return "aol.de";
		case Gazeta: return "gazeta.pl";
		case Gala: return "gala.net";
//              case Conexcol: return "conexcol.com";
		case Yandex: return "yandex.ru";
//              case MailCom: return "mail.com";
		case MailRu: return "mail.ru";
//      case Sify:  return "sify.com";
//      case Nifmail: return "nifmail.jp";
		case Bigmir: return "bigmir.net";
		case AzetSk: return "azet.sk";
		case PocztaOnet: return "poczta.onet.pl";
		case OnetEu: return "onet.eu";
//      case GmxCom: return "Gmx.com";
		case RockCom: return "rock.com";
 		default:    return "Nieobslugiwana !";
	}

}
