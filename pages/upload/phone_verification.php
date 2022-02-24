<?php
    /* 
	Template Name: Phone Verification 
	*/ 
	get_header();
	session_start();
	$error = '';
	$unconfirmed_user = get_posts(
		array(
			'showposts' => 1,
			'post_type' => 'unconfirmed_user',
			'meta_key' => 'phone',
			'meta_value' => $_GET['phone']
		)
	);
	
	if ($unconfirmed_user && $unconfirmed_user[0]) {
		$token = get_post_meta( $unconfirmed_user[0]->ID, 'token',true );
		if (!$_POST) {
			debug_to_console("Hello");
			debug_to_console($unconfirmed_user[0]->ID);
			$obj_confirm_phone = apply_filters("send_sms_unconfirmed_action", $_GET['phone']);
		}
		else {
			if ($_POST['code'] == $token)
			{
				from_unconfirmed_to_cool_user($unconfirmed_user[0]->ID);
				if (isset($_GET['redirect']) && $_GET['redirect'] == '/checkout')
					wp_redirect(get_home_url() . '/checkout'. '?add-to-cart='.$_GET['add-to-cart']);
				else if (isset($_GET['redirect']) && $_GET['redirect'] == '/code_orange')
					wp_redirect(get_home_url() . '/orange-course-choice/');
				else 
					wp_redirect(get_home_url() .'/bienvenue');
			}
			$error = 'Le code est erroné';
		}
	}
	else
		wp_redirect( get_home_url() . '/' );	
	?>
	<script>
		mixpanel.track('Start Phone Number Verification');
	</script>
<form class="formSignup" id="wp_phone_confirm_form" method="post" novalidate>
    <div id="errorPhoneVerif"></div>
    <div id="phone_number_verification">
		<h3 style="text-align: center; padding: 10px;">
			<b>
				Veuillez saisir le code de vérification à 6 chiffres que nous vous avons envoyé par SMS :
			</b>
		</h3>
		<div class="attempt_verif">(Vous avez <span id="number_of_time_sent_confirm">3</span> tentatives pour renvoyer le code)</div>
        <div style="text-align:center">
			<input class="signup_text_input" type="text" name="code" id="code_confirm_phone" required>
			<!-- <div style="margin: 30px 0px; padding: 10px;" class="row">
				<div class="col">
					<input class="digitConfirm d-block w-100 p-2 onFocus" type="text" maxLength="1" size="1" min="0" max="9" pattern="[0-9]{1}" required>    
				</div>
				<div class="col">
					<input class="digitConfirm d-block w-100 p-2 onFocus" type="text" maxLength="1" size="1" min="0" max="9" pattern="[0-9]{1}" required>    
				</div>
				<div class="col">
					<input class="digitConfirm d-block w-100 p-2 onFocus" type="text" maxLength="1" size="1" min="0" max="9" pattern="[0-9]{1}" required>    
				</div>
				<div class="col">
					<input class="digitConfirm d-block w-100 p-2 onFocus" type="text" maxLength="1" size="1" min="0" max="9" pattern="[0-9]{1}" required>    
				</div>
				<div class="col">
					<input class="digitConfirm d-block w-100 p-2 onFocus" type="text" maxLength="1" size="1" min="0" max="9" pattern="[0-9]{1}" required>    
				</div>
				<div class="col">
					<input class="digitConfirm d-block w-100 p-2 onFocus" type="text" maxLength="1" size="1" min="0" max="9" pattern="[0-9]{1}" required>    
				</div>
			</div> -->
			<div class="error"><?php echo $error != '' ? $error : '';?></div>
            <input class="form_signup_button" type="submit" id="confirm_phone_btn" name="confirm" value="Confirmer" /> 
			<div class="login_in_sign">Vous n'avez pas reçu le code ? <span id="resent_phone_verf" class="ancher_send_phone_code opacityButton">Renvoyer le code à nouveau après <span id="time_phone_verf">29 </span>secondes!</span></div>
        </div>
    </div>
</form> 

<script type="text/javascript">
jQuery(document).ready(function($) {
	$('#kezakoo-header').addClass('display_control');
	let valid = false;
	function startTimer(duration, display) {
		let seconds;
		let secondsDisplay;
        var myVar = setInterval(function() {
			if(duration >= 0) {
				seconds = parseInt(duration % 30, 10);
				secondsDisplay = seconds < 10 ? "0" + seconds : seconds;
				display.text(secondsDisplay + ' ')
			} else {
				$('.ancher_send_phone_code').removeClass('opacityButton');
				$('.ancher_send_phone_code').addClass('cursor_pointer');
				valid = true;
				clearInterval(myVar);
			}
			duration--;
		}, 1000);
    }
	display = $('#time_phone_verf');
    window.onload = function() {
        startTimer(29, display);
	};
	$('.ancher_send_phone_code').click(function (e) {
		if (valid)
		{
			mixpanel.track("Re-send Phone Verification Code");
			$('.ancher_send_phone_code').addClass('opacityButton');
			$('.ancher_send_phone_code').removeClass('cursor_pointer');
			display.text(29);
			startTimer(29, display);
			valid = false;
		}
	})
});
</script>


