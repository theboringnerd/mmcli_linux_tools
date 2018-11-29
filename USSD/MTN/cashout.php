<?php


class Cashout {

	//MTN configurations
	private $idboutton = "2";
	private $typebouton = "PAIE";
	private $_email = "wisdomnji@gmail.com";
	private $button="submit";
	private $_clP = "";
	private $_tel, $_amount;

	private $url = "https://developer.mtn.cm/OnlineMomoWeb/faces/transaction/transactionRequest.xhtml";
	
	public function __construct() {}

	public function setAmount($amount) {
		$this->_amount = $amount;	
	}

	public function setClient($_tel) {
		$this->_tel = $_tel;
	}

	public function run() {
		$post = "submit=&_clP=$this->_clP&idboutton=$this->idboutton&typebouton=$this->typebouton&_email=".urlencode($this->_email)."&_tel=$this->_tel&_amount=$this->_amount";
		return $this->sendPostData($post);
	}

	private function sendPostData($post){
		try {
			$ch = curl_init();
			if ($ch === false) {
        			throw new Exception('failed to initialize');
   			 }
			curl_setopt($ch, CURLOPT_URL, "$this->url?$post");
			curl_setopt($ch,CURLOPT_SSL_VERIFYHOST,0);
			curl_setopt($ch,CURLOPT_SSL_VERIFYPEER,0);
			curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
			$data = curl_exec($ch);
			if ($data === false) {
				throw new Exception(curl_error($ch), curl_errno($ch));
			}
			curl_close($ch);
		} catch(Exception $e) {
			trigger_error(sprintf(
			'Curl failed with error #%d: %s',
			$e->getCode(), $e->getMessage()),
			E_USER_ERROR);
		}

		return $data;
	}
}

$cashout = new Cashout;

$cashout->setAmount("100");
$cashout->setClient("652156811");

print("SERVER RETURNED\n" . $cashout->run() . "\n");
//
//var_dump($cashout->run());

?>
