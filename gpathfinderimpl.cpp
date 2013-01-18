/*
 * File: gpathfinderimpl.cpp
 * -------------------------
 * This function implements the template functions AddButton
 * and AddClickListener.  C++ requires that this code be
 * available at the time the interface is read, but clients
 * are not expected to look at this code.
 */

#ifdef _gpathfinder_h

/*
 * Implementation notes: callback classes
 * --------------------------------------
 * The task of storing a callback function that is triggered
 * at the time of a mouse or button click is more difficult that
 * it might at first appear.  The complexity arises from the fact
 * that the ClientDataType parameter to the callback function is
 * supplied at the time the button is declared but used in the
 * implementation of PathfinderEventLoop, which no longer has
 * access to the ClientDataType information.  To solve this
 * problem, the callback functions and the address of the
 * client data object are stored in a template subclass of
 * a standard base class containing a virtual apply method.
 * When PathfinderEventLoop wants to trigger the callback
 * action, it calls the apply method of the base class, which
 * then invokes the specific code for the relevant subclass
 * that knows the identity of ClientDataType.  The base classes
 * are also used on their own to implement the callbacks when
 * no ClientDataType value is supplied.
 */

class ButtonCallbackBase {
public:
	ButtonCallbackBase();
	ButtonCallbackBase(void (*actionFn)());
	virtual ~ButtonCallbackBase();
	virtual void apply();

private:
	void (*actionFn)();
};

template <typename ClientDataType>
class ButtonCallback : public ButtonCallbackBase {
public:
	ButtonCallback(void (*actionFn)(ClientDataType & data), ClientDataType *pdata);
	virtual ~ButtonCallback();
	virtual void apply();

private:
	void (*actionFn)(ClientDataType & data);
	ClientDataType *pdata;
};

template <typename ClientDataType>
ButtonCallback<ClientDataType>::ButtonCallback(void (*actionFn)(ClientDataType & data),
                                               ClientDataType *pdata) {
	this->actionFn = actionFn;
	this->pdata = pdata;
}

template <typename ClientDataType>
ButtonCallback<ClientDataType>::~ButtonCallback() {
	/* Empty */
}

template <typename ClientDataType>
void ButtonCallback<ClientDataType>::apply() {
	actionFn(*pdata);
}

class ClickCallbackBase {
public:
	ClickCallbackBase();
	ClickCallbackBase(void (*clickFn)(pointT pt));
	virtual ~ClickCallbackBase();
	virtual void apply(pointT pt);

private:
	void (*clickFn)(pointT pt);
};

template <typename ClientDataType>
class ClickCallback : public ClickCallbackBase {
public:
	ClickCallback(void (*clickFn)(pointT pt, ClientDataType & data),
	              ClientDataType *pdata);
	virtual ~ClickCallback();
	virtual void apply(pointT pt);

private:
	void (*clickFn)(pointT pt, ClientDataType & data);
	ClientDataType *pdata;
};

template <typename ClientDataType>
ClickCallback<ClientDataType>::ClickCallback(void (*clickFn)(pointT pt, ClientDataType & data),
										     ClientDataType *pdata) {
	this->clickFn = clickFn;
	this->pdata = pdata;
}

template <typename ClientDataType>
ClickCallback<ClientDataType>::~ClickCallback() {
	/* Empty */
}

template <typename ClientDataType>
void ClickCallback<ClientDataType>::apply(pointT pt) {
	clickFn(pt, *pdata);
}

/* Prototypes for functions in gpathfinder.cpp */

void AddButton(string name, ButtonCallbackBase *callback);
void DefineClickListener(ClickCallbackBase *callback);

/*
 * Implementation notes: AddButton, DefineClickListener
 * ----------------------------------------------------
 * These template functions create the appropriate callback
 * structure and then call a non-template function in
 * gpathfinder.cpp that works with that with the base class
 * of that object and therefore no longer requires the
 * overhead of C++'s template mechanism.
 */

template <typename ClientDataType>
void AddButton(string name,
               void (*actionFn)(ClientDataType & data),
               ClientDataType & data) {
	AddButton(name, new ButtonCallback<ClientDataType>(actionFn, &data));
}

template <typename ClientDataType>
void DefineClickListener(void (*clickFn)(pointT pt, ClientDataType & data),
                         ClientDataType & data) {
	DefineClickListener(new ClickCallback<ClientDataType>(clickFn, &data));
}

#endif

